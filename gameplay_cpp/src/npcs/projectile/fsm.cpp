#include "fsm.h"

#include "projectile.h"

#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "npcs", "projectile"

using namespace ProjectileFsm;

Launched::Return ProjectileFsm::Launched::enter(StateContext& context) {
	m_timer = memnew(godot::Timer);
	ASSERT_NOTNULL(m_timer)

	context.owner->add_child(m_timer);
	m_timer->connect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
	m_timer->start(GETPARAM_D("lifetime"));

	return {};
}

Launched::Return ProjectileFsm::Launched::exit(StateContext& context) {
	m_timer->stop();
	m_timer->disconnect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
	context.owner->remove_child(m_timer);
	m_timer->queue_free();

	return {};
}

Launched::Return Launched::notification(StateContext& context, int what) {
	switch (what) {
		case ENotifications::PARRIED: {
		}
		case ENotifications::ATTACKED: {
			auto deathParticles = context.owner->getDeathParticles();
			// m_particles->restart();
			ASSERT(deathParticles.is_valid())
			godot::Node* root = context.owner->get_node<godot::Node>(nodePaths::root);
			ASSERT_NOTNULL(root)

			auto* particles = context.owner->cast_to<godot::GPUParticles3D>(deathParticles->instantiate());
			root->add_child(particles);

			particles->set_transform(context.owner->get_global_transform());
			particles->restart();
			context.owner->queue_free();
		}
		case ENotifications::DESTROY: {
			context.owner->queue_free();
			break;
		}
		default:
			break;
	}

	return {};
}
