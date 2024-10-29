
#ifndef GD_CHARACTER_PLAYERFSM_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERFSM_PLUGIN_GAMEPLAY_H

#include <functional>

#include <core/core.h>
#include <core/fsm.hpp>

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class PlayerState;
class PlayerFSM;
class GrappleComponent;
class ParryInstigatorComponent;
class InputComponent;
namespace godot {
	class AudioStreamPlayer3D;
	class GPUParticles3D;
}

constexpr float PLAYER_CHARACTER_HEIGHT = 2.0f;
constexpr float PLAYER_CHARACTER_HALFHEIGHT = PLAYER_CHARACTER_HEIGHT / 2.f;

// Data som en playerstate alltid er interessert i
//  Forer inn en ny struct som dette i hver _physics_process
struct StatePhysicsContext {
	bool is_on_ground;
	Vector3 position;
	Vector3 velocity;

	Vector3 get_gravity_center() const {
		return Vector3(position.x, position.y + PLAYER_CHARACTER_HALFHEIGHT, position.z);
	}
};
struct StateGrappleContext {
	GrappleComponent* instigator = nullptr;
	GrappleComponent* target = nullptr;
};
struct AudioVisualContext {
	godot::AudioStreamPlayer3D* audio = nullptr; // todo, audiocomponent for streaming multiple sounds from context?
	godot::GPUParticles3D* particles = nullptr;
};

struct StateContext {
	StateGrappleContext grapple;
	InputComponent* input = nullptr;
	ParryInstigatorComponent* parry = nullptr;
	StatePhysicsContext physics;
	AudioVisualContext audioVisual;
};

class PlayerState : public State<StateContext, PlayerState> {
	// BUG: regarding guarantee one frame processing?

public:
	// default overrides for PlayerState
	virtual bool canEnter() const override { return true; }
	virtual Return enter(StateContext& context) override {
		LOG(DEBUG, "Entering state ", getName())
		return {};
	}
	virtual Return exit(StateContext& context) override { return {}; }
	virtual Return process(StateContext& context, float delta) override { return {}; }
	virtual Return physicsProcess(StateContext& context, float delta) override { return {}; }
	virtual Return handleInput(StateContext& context, float delta) override { return {}; }
	virtual Return deferredActions(StateContext& context) override { return {}; }

#define PLAYER_STATE_IMPL(CLASSNAME)                                                                                   \
	typedef PlayerState Super;                                                                                         \
	virtual const char* getName() override{ return #CLASSNAME;                                                         \
	}
};

class PlayerFSM : public Fsm<StateContext, PlayerState, PlayerState::Return> {};

#endif // GD_CHARACTER_PLAYERFSM_PLUGIN_GAMEPLAY_H