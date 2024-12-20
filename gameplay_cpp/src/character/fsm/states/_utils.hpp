#pragma once

#include "../typedefs.hpp"

#include <managers/inputManager.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/kinematic_collision3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <configHandler.h>

#ifdef CONFIG_PREFIX
#undef CONFIG_PREFIX
#endif
#define CONFIG_PREFIX "player"

namespace fsm::player::utils {

inline void movementAcceleration(Context& context, float acceleration, float deceleration, float delta) {
	// direction
	if (context.input->m_inputRaw.abs() > godot::Vector2()) {
		context.physics.velocity.x = godot::Math::move_toward(context.physics.velocity.x,
			context.input->m_inputCameraRelative.x * GETPARAM_F("walkSpeed"),
			acceleration * delta);
		context.physics.velocity.z = godot::Math::move_toward(context.physics.velocity.z,
			context.input->m_inputCameraRelative.y * GETPARAM_F("walkSpeed"),
			acceleration * delta);
	}
	else {
		context.physics.velocity.x = godot::Math::move_toward(context.physics.velocity.x, 0.0f, deceleration * delta);
		context.physics.velocity.z = godot::Math::move_toward(context.physics.velocity.z, 0.0f, deceleration * delta);
	}
}

inline void moveSlideOwner(Context& context) {
	auto* owner = context.owner;
	owner->set_velocity(context.physics.velocity);
	context.physics.collided = owner->move_and_slide();
}

typedef struct {
	uint32_t layer;
	uint32_t mask;
} CollisionLayerMask;

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

inline void revertRigidbodyCollisionSlide(Context& context) {
	auto* owner = context.owner;
	for (int i = 0; i < owner->get_slide_collision_count(); i++) {
		auto col = owner->get_slide_collision(i);
		if (godot::Object::cast_to<godot::RigidBody3D>(col->get_collider())) {
			const auto pos = owner->get_global_position();
			// const auto adjustment = col->get_travel() + col->get_remainder();
			const auto adjustment = col->get_travel();
			owner->set_global_position(pos - adjustment);
			owner->set_velocity(Vector3());
		}
	}
}

} //namespace fsm::player::utils

#undef CONFIG_PREFIX
