#pragma once

#include "../typedefs.hpp"
#include <npcs/projectile/projectile.h>
#include <configparams.hpp>

#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/node.hpp>

namespace fsm::projectile::utils {

inline void death(Context& context) {
	auto deathParticles = context.owner->getDeathParticles();
	ASSERT(deathParticles.is_valid())
	godot::Node* root = context.owner->get_node<godot::Node>(nodePaths::root);
	ASSERTNN(root)

	auto* particles = context.owner->cast_to<godot::GPUParticles3D>(deathParticles->instantiate());
	root->add_child(particles);

	particles->set_transform(context.owner->get_global_transform());
	particles->restart();
	context.owner->queue_free();
}

} //namespace fsm::projectile::utils