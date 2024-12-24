#pragma once

#include <core/core.hpp>
#include "../../utils/circularbuffer.h"
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>

class ComponentAnimation;
class ComponentAttackInstigator;
class ComponentGrappleInstigator;
class ComponentParryInstigator;
class InputManager;

namespace godot {
class AudioStreamPlayer3D;
class GPUParticles3D;
class Node3D;
class RigidBody3D;
} //namespace godot

namespace fsm::player {

constexpr float PLAYER_CHARACTER_HEIGHT = 2.0f; // Magic number, guesswork
constexpr float PLAYER_CHARACTER_HALFHEIGHT = PLAYER_CHARACTER_HEIGHT / 2.f;
constexpr int STATES_BUFFER_SIZE = 20;

struct PhysicsContext {
	bool isOnGround;
	bool collided;
	godot::Vector3 position;
	godot::Vector3 velocity;
	godot::Vector3 movement;

	godot::Vector3 getGravityCenter() const {
		return godot::Vector3(position.x, position.y + PLAYER_CHARACTER_HALFHEIGHT, position.z);
	}

	// godot::Vector3 getVelocityDir2D() const {
	// 	return godot::Vector3(velocity.x, 0, velocity.z).normalized();
	// }
};
struct AudioVisualContext {
	godot::AudioStreamPlayer3D* audio = nullptr; // todo, audiocomponent for streaming multiple sounds from context?
	godot::GPUParticles3D* particles = nullptr; // todo, same as above?
};

struct TOnGroundState {};
struct TInAirState {};
struct TPreGrappleLaunchState {};
struct TGrappleLaunchState {};
struct TParryPreState {};
struct TParryPostState {};
struct TParryJumpState {};
struct TAttackState {};
using TState = std::variant<std::monostate, TOnGroundState, TInAirState, TPreGrappleLaunchState, TGrappleLaunchState,
	TParryPreState, TParryPostState, TParryJumpState, TAttackState>;

struct Context {
	godot::CharacterBody3D* owner = nullptr;
	ComponentAttackInstigator* attack = nullptr;
	ComponentAnimation* anim = nullptr;
	ComponentGrappleInstigator* grapple = nullptr;
	InputManager* input = nullptr;
	ComponentParryInstigator* parry = nullptr;
	PhysicsContext physics;
	AudioVisualContext audioVisual;
	CircularBuffer<TState>* states = nullptr;
};

inline godot::String stateName(TState state) {
	godot::String out;
	std::visit(
		overloaded{
			[&](TAttackState) { out="atttackState"; },
			[&](TOnGroundState) { out="onGroundState"; },
			[&](TGrappleLaunchState) { out="grappleLaunchState"; },
			[&](TInAirState) { out="inAirState"; },
			[&](TParryPreState) { out="parryState"; },
			[&](TParryPostState) { out="parryPostState"; },
			[&](TParryJumpState) { out="parryJumpState"; },
			[&](TPreGrappleLaunchState) { out="preGrappleLaunchState"; },
			[&](std::monostate) { ASSERT(false); },
		},
		state);
	return out;
};

inline void printStates(CircularBuffer<TState>* states) {
	godot::String out="";
	for (int i=0; i<5; i++ ) {
		out+=stateName(states->get(i)) + "<-";
	}
	LOG(DEBUG, "states:", out);
}

class BaseState {
public:
	virtual bool canEnter() const {
		return true;
	}
	virtual TState getType() const = 0;
	virtual TState enter(Context& context) = 0;
	virtual TState exit(Context& context) = 0;
	virtual TState process(Context& context, float delta) = 0;
	virtual TState physicsProcess(Context& context, float delta) = 0;
	virtual TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) = 0;
	virtual TState handleInput(Context& context, float delta) = 0;
};

} //namespace fsm::player
