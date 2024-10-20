#ifndef GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H

#include <character/playerfsm.h>

class PlayerOnGroundState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerOnGroundState)

	Return enter(StateContext& context) override;

	Return physicsProcess(StateContext& context, float delta) override;
	Return handleInput(StateContext& context, float delta) override;
};

class PlayerInAirState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerInAirState)

	Return physicsProcess(StateContext& context, float delta) override;
	Return handleInput(StateContext& context, float delta) override;
};

class PlayerPreGrappleLaunchState : public PlayerState {
public:
	Timestamp timestamp;

public:
	PLAYER_STATE_IMPL(PlayerPreGrappleLaunchState)

	Return enter(StateContext& context) override;
};

class PlayerGrappleLaunchState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerGrappleLaunchState)

	Return enter(StateContext& context) override;
};

class PlayerParryState : public PlayerState {
private:
	Timestamp m_enterTimestamp;
	Timestamp m_exitTimestamp;

public:
	PLAYER_STATE_IMPL(PlayerParryState)

	bool canEnter() const;
	Return enter(StateContext& context) override;
	Return exit(StateContext& context) override;
	Return physicsProcess(StateContext& context, float delta) override;
	Return handleInput(StateContext& context, float delta) override;
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
	PlayerParryState parrystate;

public:
	static PlayerStateBank& get() {
		static PlayerStateBank statebank;
		return statebank;
	}
	template <class T>
	PlayerState* state() {
		static_assert(std::is_base_of_v<PlayerState, T>, "PlayerStateBank can only handle PlayerStates");
		if constexpr (std::is_same_v<T, PlayerOnGroundState>) { return &ongroundstate; }
		if constexpr (std::is_same_v<T, PlayerInAirState>) { return &inairstate; }
		if constexpr (std::is_same_v<T, PlayerPreGrappleLaunchState>) { return &pregrapplelaunchstate; }
		if constexpr (std::is_same_v<T, PlayerGrappleLaunchState>) { return &grapplelaunchstate; }
		if constexpr (std::is_same_v<T, PlayerParryState>) { return &parrystate; }
	}
};

#endif // GD_CHARACTER_PLAYERSTATES_PLUGIN_GAMEPLAY_H