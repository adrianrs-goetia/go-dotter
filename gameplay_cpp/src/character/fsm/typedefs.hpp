#pragma once

#include <core/core.hpp>

class GrappleTargetComponent;
class GrappleInstigatorComponent;
class ParryInstigatorComponent;
class AttackComponent;
class AnimationComponent;
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
	godot::Vector3 position;
	godot::Vector3 velocity;

	godot::Vector3 get_gravity_center() const {
		return godot::Vector3(position.x, position.y + PLAYER_CHARACTER_HALFHEIGHT, position.z);
	}
};
struct AudioVisualContext {
	godot::AudioStreamPlayer3D* audio = nullptr; // todo, audiocomponent for streaming multiple sounds from context?
	godot::GPUParticles3D* particles = nullptr; // todo, same as above?
};

struct Context {
	AttackComponent* attack = nullptr;
	AnimationComponent* anim = nullptr;
	GrappleInstigatorComponent* grapple = nullptr;
	InputManager* input = nullptr;
	ParryInstigatorComponent* parry = nullptr;
	PhysicsContext physics;
	AudioVisualContext audioVisual;
};

struct TOnGroundState {};
struct TInAirState {};
struct TPreGrappleLaunchState {};
struct TGrappleLaunchState {};
struct TParryState {};
struct TAttackState {};
using TState = std::variant<std::monostate, TOnGroundState, TInAirState, TPreGrappleLaunchState, TGrappleLaunchState,
	TParryState, TAttackState>;

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
};

} //namespace fsm::player
