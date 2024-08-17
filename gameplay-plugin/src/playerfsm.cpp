#include <character/playerfsm.h>

PlayerState::PlayerState(bool one_frame) : m_guarantee_one_frame(one_frame) {}
PlayerState::PlayerState() {}

StateReturn PlayerState::enter_state(StateContext* context) {
	printf("Entering state %s \n", get_class_name());
	return {};
}

void PlayerState::exit_state(StateContext* context) {}

void PlayerFSM::deferred_actions(StateContext* context) {
	if (m_current_state) {
		m_current_state->deferred_actions(context);
	}
}

void PlayerFSM::_exit_tree() {
	if (m_current_state) {
		delete m_current_state;
		m_current_state = nullptr;
	}
	ASSERT(m_current_state == nullptr, "");
}

void PlayerFSM::process(StateContext* context, real_t delta) {
	if (m_current_state) {
		_process_state(context, m_current_state->process(context, delta));
	}
}

void PlayerFSM::physics_process(StateContext* context, real_t delta) {
	if (m_current_state) {
		_process_state(context, m_current_state->physics_process(context, delta));
	}
}

void PlayerFSM::handle_input(StateContext* context, real_t delta) {
	if (m_current_state) {
		_process_state(context, m_current_state->handle_input(context, delta));
	}
}

void PlayerFSM::_process_state(StateContext* context, StateReturn state_return) {
	ASSERT(m_current_state != nullptr, "");
	switch (state_return.ret_enum) {
		case EStateReturn::CONTINUE:
			break;
		case EStateReturn::NEW_STATE:
			if (!state_return.new_state) {
				printf("%s new state was nullptr. Abort changing state", __FUNCTION__);
				break;
			}
			m_current_state->exit_state(context);
			delete m_current_state;
			m_current_state = nullptr;
			m_current_state = state_return.new_state;
			_process_state(context, m_current_state->enter_state(context));
			break;
		default:
			break;
	}
}