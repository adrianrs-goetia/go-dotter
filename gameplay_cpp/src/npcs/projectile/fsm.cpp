#include "fsm.h"

#include "projectile.h"

#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "npcs", "projectile"

using namespace godot;

namespace ProjectileFsm {

namespace utils {

void death(StateContext& context) {
	auto deathParticles = context.owner->getDeathParticles();
	ASSERT(deathParticles.is_valid())
	Node* root = context.owner->get_node<Node>(nodePaths::root);
	ASSERT_NOTNULL(root)

	auto* particles = context.owner->cast_to<GPUParticles3D>(deathParticles->instantiate());
	root->add_child(particles);

	particles->set_transform(context.owner->get_global_transform());
	particles->restart();
	context.owner->queue_free();
}

} //namespace utils

// ###############################################
// ## LAUNCHED
// ###############################################
Return ProjectileFsm::Launched::enter(StateContext& context) {
	m_timer = memnew(Timer);
	ASSERT_NOTNULL(m_timer)

	context.owner->add_child(m_timer);
	m_timer->connect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
	m_timer->start(GETPARAM_D("lifetime"));

	return {};
}

Return ProjectileFsm::Launched::exit(StateContext& context) {
	ASSERT_NOTNULL(m_timer)
	m_timer->stop();
	m_timer->disconnect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
	context.owner->remove_child(m_timer);
	m_timer->queue_free();

	return {};
}

Return ProjectileFsm::Launched::handleExternalAction(StateContext& context, const ExternalAction& action) {
	if (std::holds_alternative<AttackInstance>(action)) {
		utils::death(context);
	}
	else if (std::holds_alternative<ParryInstance>(action)) {
		context.owner->set_linear_velocity(Vector3(0, 3, 0));
		return TParried{};
	}
	return {};
}

// ###############################################
// ## PARRIED
// ###############################################
Return ProjectileFsm::Parried::enter(StateContext& context) {
	return {};
}

Return ProjectileFsm::Parried::exit(StateContext& context) {
	return {};
}

Return Parried::physicsProcess(StateContext& context, float delta) {
	if (context.owner->isOnGround()) {
		utils::death(context);
	}
	return {};
}

Return ProjectileFsm::Parried::handleExternalAction(StateContext& context, const ExternalAction& action) {
	if (std::holds_alternative<AttackInstance>(action)) {
		auto& attackInstance = std::get<AttackInstance>(action);
		const Vector3 dir = attackInstance.getDirection();
		context.owner->set_linear_velocity(dir * attackInstance.attackStrength);
		return TLaunched{};
	}
	return {};
}

} //namespace ProjectileFsm