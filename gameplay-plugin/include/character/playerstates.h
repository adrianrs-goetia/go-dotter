#ifndef GD_CHARACTER_PLAYERSTATES_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_CHARACTER_PLAYERSTATES_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <character/playerfsm.h>

// Forward declare
class PlayerOnGroundState;
class PlayerInAirState;

class PlayerOnGroundState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerOnGroundState)

	StateReturn enter_state() override;
	StateReturn process(real_t delta) override { return {}; }
	StateReturn physics_process(real_t delta) override;
	StateReturn handle_input(real_t delta) override;
};

class PlayerInAirState : public PlayerState {
public:
	PLAYER_STATE_IMPL(PlayerInAirState)

	StateReturn process(real_t delta) override { return {}; }
	StateReturn physics_process(real_t delta) override;
	StateReturn handle_input(real_t delta) override { return {}; }
};

#endif // GD_CHARACTER_PLAYERSTATES_PLUGIN_STEIKEMANNGAMEPLAY_H