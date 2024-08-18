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
	Timestamp timestamp;

public:
	PLAYER_STATE_IMPL(PlayerPreGrappleLaunchState)

	StateReturn enter_state(StateContext* context) override;
	StateReturn process(StateContext* context, float delta) override { return {}; }
	StateReturn physics_process(StateContext* context, float delta) override { return {}; }
	StateReturn handle_input(StateContext* context, float delta) override { return {}; }
};

class PlayerGrappleLaunchState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerGrappleLaunchState)

	StateReturn enter_state(StateContext* context) override;
	StateReturn process(StateContext* context, float delta) override { return {}; }
	StateReturn physics_process(StateContext* context, float delta) override { return {}; }
	StateReturn handle_input(StateContext* context, float delta) override { return {}; }
};

/*
 * Static PlayerStateBank with memory baked into the binary.
 * Avoid allocating memory on heap when switching between states;
 * while maintaining potential persistence data about the player states.
 *
 * Assumed:
 *	single threaded state handling
 *	only used within states
 */
class PlayerStateBank {
	PlayerOnGroundState ongroundstate;
	PlayerInAirState inairstate;
	PlayerPreGrappleLaunchState pregrapplelaunchstate;
	PlayerGrappleLaunchState grapplelaunchstate;

public:
	static PlayerStateBank& get() {
		static PlayerStateBank statebank;
		return statebank;
	}
	template <class T> PlayerState* state(bool guarantee_one_frame_processing = false) {
		static_assert(std::is_base_of_v<PlayerState, T>, "PlayerStateBank can only handle PlayerStates");
		if (std::is_same_v<T, PlayerOnGroundState>) {
			ongroundstate.init(guarantee_one_frame_processing);
			return &ongroundstate;
		}
		if (std::is_same_v<T, PlayerInAirState>) {
			inairstate.init(guarantee_one_frame_processing);
			return &inairstate;
		}
		if (std::is_same_v<T, PlayerPreGrappleLaunchState>) {
			pregrapplelaunchstate.init(guarantee_one_frame_processing);
			return &pregrapplelaunchstate;
		}
		if (std::is_same_v<T, PlayerGrappleLaunchState>) {
			grapplelaunchstate.init(guarantee_one_frame_processing);
			return &grapplelaunchstate;
		}
	}
};

#endif // GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H