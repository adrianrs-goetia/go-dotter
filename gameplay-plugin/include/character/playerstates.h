#ifndef GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H

#include <character/playerfsm.h>

// // Forward declare
// class PlayerOnGroundState;
// class PlayerInAirState;

class PlayerOnGroundState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerOnGroundState)

	StateReturn enter_state(StateContext* context) override;
	StateReturn process(StateContext* context, float delta) override { return {}; }
	StateReturn physics_process(StateContext* context, float delta) override;
	StateReturn handle_input(StateContext* context, float delta) override;
};

class PlayerInAirState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerInAirState)

	StateReturn process(StateContext* context, float delta) override { return {}; }
	StateReturn physics_process(StateContext* context, float delta) override;
	StateReturn handle_input(StateContext* context, float delta) override;
};

class PlayerPreGrappleLaunchState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerPreGrappleLaunchState)

	StateReturn enter_state(StateContext* context) override;
	StateReturn process(StateContext* context, float delta) override { return {}; }
	StateReturn physics_process(StateContext* context, float delta) override { return {}; }
	StateReturn handle_input(StateContext* context, float delta) override { return {}; }
};

class PlayerGrappleLaunchState : public PlayerState {
public:
	

public:
	PLAYER_STATE_IMPL(PlayerGrappleLaunchState)

	StateReturn enter_state(StateContext* context) override;
	StateReturn process(StateContext* context, float delta) override { return {}; }
	StateReturn physics_process(StateContext* context, float delta) override { return {}; }
	StateReturn handle_input(StateContext* context, float delta) override { return {}; }
};

#endif // GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H