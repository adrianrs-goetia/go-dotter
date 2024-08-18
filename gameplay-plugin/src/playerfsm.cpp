#include <character/playerfsm.h>

void PlayerState::init(bool guarantee_one_frame_processing) {
	m_guarantee_one_frame_processing = guarantee_one_frame_processing;
}

StateReturn PlayerState::enter_state(StateContext* context) {
	LOG(DEBUG, "Entering state ", get_class_name());
	return {};
}

void PlayerState::exit_state(StateContext* context) {}

void PlayerFSM::deferred_actions(StateContext* context) {
	if (m_current_state) { m_current_state->deferred_actions(context); }
}

void PlayerFSM::_exit_tree() {
	if (m_current_state) {
		delete m_current_state;
		m_current_state = nullptr;
	}
	ASSERT(m_current_state == nullptr, "");
}

void PlayerFSM::process(StateContext* context, float delta) {
	if (m_current_state) { _process_state(context, m_current_state->process(context, delta)); }
}

void PlayerFSM::physics_process(StateContext* context, float delta) {
	if (m_current_state) { _process_state(context, m_current_state->physics_process(context, delta)); }
}

void PlayerFSM::handle_input(StateContext* context, float delta) {
	if (m_current_state) { _process_state(context, m_current_state->handle_input(context, delta)); }
}

void PlayerFSM::_process_state(StateContext* context, StateReturn state_return) {
	ASSERT(m_current_state != nullptr, "");
	switch (state_return.ret_enum) {
		case EStateReturn::CONTINUE: break;
		case EStateReturn::NEW_STATE:
			ASSERT(state_return.new_state != nullptr, "");
			m_current_state->exit_state(context);
			m_current_state = state_return.new_state;
			_process_state(context, m_current_state->enter_state(context));
			break;
		default: break;
	}
}