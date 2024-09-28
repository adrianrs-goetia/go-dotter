#ifndef GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H

#include <character/playerfsm.h>

class PlayerOnGroundState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerOnGroundState)

	Return enter(StateContext* context) override;

	Return physics_process(StateContext* context, float delta) override;
	Return handle_input(StateContext* context, float delta) override;
};

class PlayerInAirState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerInAirState)

	Return physics_process(StateContext* context, float delta) override;
	Return handle_input(StateContext* context, float delta) override;
};

class PlayerPreGrappleLaunchState : public PlayerState {
public:
	Timestamp timestamp;

public:
	PLAYER_STATE_IMPL(PlayerPreGrappleLaunchState)

	Return enter(StateContext* context) override;
};

class PlayerGrappleLaunchState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerGrappleLaunchState)

	Return enter(StateContext* context) override;
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
	template <class T>
	PlayerState* state() {
		static_assert(std::is_base_of_v<PlayerState, T>, "PlayerStateBank can only handle PlayerStates");
		if (std::is_same_v<T, PlayerOnGroundState>) { return &ongroundstate; }
		if (std::is_same_v<T, PlayerInAirState>) { return &inairstate; }
		if (std::is_same_v<T, PlayerPreGrappleLaunchState>) { return &pregrapplelaunchstate; }
		if (std::is_same_v<T, PlayerGrappleLaunchState>) { return &grapplelaunchstate; }
	}
};

#endif // GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H