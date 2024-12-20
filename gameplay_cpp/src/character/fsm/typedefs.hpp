#pragma once

#include <core/core.hpp>

class ComponentGrappleInstigator;
class ComponentParryInstigator;
class ComponentAttackInstigator;
class ComponentAnimation;
class InputManager;

namespace godot {
class Node3D;
class AudioStreamPlayer3D;
class GPUParticles3D;
} //namespace godot

namespace fsm::player {

constexpr float PLAYER_CHARACTER_HEIGHT = 2.0f; // Magic number, guesswork
constexpr float PLAYER_CHARACTER_HALFHEIGHT = PLAYER_CHARACTER_HEIGHT / 2.f;

struct PhysicsContext {
	bool isOnGround;
	bool collided;
	godot::Vector3 position;
	godot::Vector3 velocity;

	godot::Vector3 get_gravity_center() const {
		return godot::Vector3(position.x, position.y + PLAYER_CHARACTER_HALFHEIGHT, position.z);
	}

	godot::Vector3 getVelocityDir2D() const {
		return godot::Vector3(velocity.x, 0, velocity.z).normalized();
	}
};
struct AudioVisualContext {
	godot::AudioStreamPlayer3D* audio = nullptr; // todo, audiocomponent for streaming multiple sounds from context?
	godot::GPUParticles3D* particles = nullptr; // todo, same as above?
};

struct Context {
	ComponentAttackInstigator* attack = nullptr;
	ComponentAnimation* anim = nullptr;
	ComponentGrappleInstigator* grapple = nullptr;
	InputManager* input = nullptr;
	ComponentParryInstigator* parry = nullptr;
	PhysicsContext physics;
	AudioVisualContext audioVisual;
};

struct TOnGroundState {};
struct TInAirState {};
struct TPreGrappleLaunchState {};
struct TGrappleLaunchState {};
struct TParryState {};
struct TParryPostState {};
struct TParryJumpState {};
struct TAttackState {};
using TState = std::variant<std::monostate, TOnGroundState, TInAirState, TPreGrappleLaunchState, TGrappleLaunchState,
	TParryState, TParryPostState, TParryJumpState, TAttackState>;

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
	virtual TState handleInput(Context& context, float delta) = 0;
	virtual TState deferredPhysicsProcess(Context& context, float delta) = 0;
};

} //namespace fsm::player
