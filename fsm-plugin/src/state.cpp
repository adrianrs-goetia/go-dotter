#include "state.h"

#include <godot_cpp/core/class_db.hpp>
#include "playerstate.h"


void FSM::_enter_tree()
{
    printf("entering tree FSM ###\n");
    _current_state = new IdleState;
    assert(_current_state != nullptr);
}

void FSM::_exit_tree()
{
    printf("exiting tree FSM!\n");
    delete _current_state;
    _current_state = nullptr;
    assert(_current_state == nullptr);
}

void FSM::_process(double delta)
{
    if (_current_state)
    {
        process_state(_current_state->process(delta));
    }
}

void FSM::_unhandled_input(const godot::Ref<godot::InputEvent> &p_event)
{
    if (_current_state)
    {
        process_state(_current_state->handle_input(p_event));
    }
}

void FSM::process_state(State::StateReturn state_return)
{
    assert(_current_state != nullptr);
    switch (state_return.ret_enum)
    {
    case State::EnumStateReturn::NONE:
    case State::EnumStateReturn::CONTINUE:
        break;
    case State::EnumStateReturn::NEW_STATE:
        if (!state_return.new_state)
        {
            printf("__FUNCTION__ new state was nullptr. Abort changing state");
            break;
        }
        _current_state->exit_state();
        delete _current_state;
        _current_state = nullptr;
        _current_state = state_return.new_state;
        process_state(_current_state->enter_state());
        break;
    default:
        break;
    }
}
