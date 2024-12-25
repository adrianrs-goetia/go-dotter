#pragma once

#include "../typedefs.hpp"

#include <managers/inputManager.h>
#include <components/animation.hpp>

#include <godot_cpp/classes/physics_material.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <configparams.hpp>

namespace fsm::player::utils {

inline void movementAcceleration(Context& context, float acceleration, float deceleration, float delta) {
	auto& vec = context.physics.movement;
	// direction
	// if (context.input->m_inputRaw.abs() > godot::Vector2()) {
	float desiredSpeed = context.input->isActionHeld(EInputAction::SPRINT) ? ConfigParam::Player::sprintSpeed()
																		   : ConfigParam::Player::walkSpeed();

	if (context.input->m_inputRaw.length_squared() > 0.2f) {
		vec.x = godot::Math::move_toward(vec.x, context.input->m_inputCameraRelative.x * desiredSpeed, acceleration);
		vec.z = godot::Math::move_toward(vec.z, context.input->m_inputCameraRelative.y * desiredSpeed, acceleration);
	}
	else {
		vec.x = godot::Math::move_toward(vec.x, 0.0f, deceleration);
		vec.z = godot::Math::move_toward(vec.z, 0.0f, deceleration);
	}
}

struct CollisionLayerMask {
	uint32_t layer;
	uint32_t mask;
};

inline CollisionLayerMask disableCollision(Context& context) {
	CollisionLayerMask clm{ context.owner->get_collision_layer(), context.owner->get_collision_mask() };
	context.owner->set_collision_layer(0);
	context.owner->set_collision_mask(1); // Not masking out static world
	return clm;
}

inline void enableCollision(Context& context, const CollisionLayerMask& clm) {
	context.owner->set_collision_layer(clm.layer);
	context.owner->set_collision_mask(clm.mask);
}

inline bool isOnFloor(const godot::PhysicsDirectBodyState3D& state) {
	for (int i = 0; i < state.get_contact_count(); i++) {
		auto normal = state.get_contact_local_normal(i);
		// On floor
		if (g_up.dot(normal) > ConfigParam::Player::floorMaxAngle()) {
			return true;
		}
	}
	return false;
}

// Assumes input is always along the horizontal plane
inline godot::Vector3 getInputOrForward(const Context& c) {
	const auto dir = c.input->getInputRelative3d();
	if (dir.length_squared() < 0.2f) {
		return c.anim->m_animRoot->get_global_basis().get_column(2);
	}
	return dir;
}

} //namespace fsm::player::utils
