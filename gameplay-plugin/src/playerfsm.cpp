#include <character/playerfsm.h>

PlayerState::PlayerState(PlayerState* state, bool one_frame) :
		m_guarantee_one_frame(one_frame), m_context(state->m_context) {}
PlayerState::PlayerState(StateContext* context) : m_context(context) {}

StateReturn PlayerState::enter_state() {
	printf("Entering state %s \n", get_class_name());
	return {};
}

void PlayerState::exit_state() {}

void PlayerFSM::init(StateContext* context) {
	_enter_tree();
	m_context = context;
	ASSERT(m_context != nullptr, "");
}
void PlayerFSM::uninit() {
	_exit_tree();
	m_context = nullptr;
}

void PlayerFSM::deferred_actions() {
	if (m_current_state) {
		m_current_state->deferred_actions();
	}
}

void PlayerFSM::_enter_tree() {}

void PlayerFSM::_exit_tree() {
	if (m_current_state) {
		delete m_current_state;
		m_current_state = nullptr;
	}
	ASSERT(m_current_state == nullptr, "");
}

void PlayerFSM::process(real_t delta) {
	if (m_current_state) {
		_process_state(m_current_state->process(delta));
	}
}

void PlayerFSM::physics_process(real_t delta) {
	if (m_current_state) {
		_process_state(m_current_state->physics_process(delta));
	}
}

void PlayerFSM::handle_input(real_t delta) {
	if (m_current_state) {
		_process_state(m_current_state->handle_input(delta));
	}
}

void PlayerFSM::_process_state(StateReturn state_return) {
	ASSERT(m_current_state != nullptr, "");
	switch (state_return.ret_enum) {
		case EStateReturn::CONTINUE:
			break;
		case EStateReturn::NEW_STATE:
			if (!state_return.new_state) {
				printf("%s new state was nullptr. Abort changing state", __FUNCTION__);
				break;
			}
			m_current_state->exit_state();
			delete m_current_state;
			m_current_state = nullptr;
			m_current_state = state_return.new_state;
			_process_state(m_current_state->enter_state());
			break;
		default:
			break;
	}
}