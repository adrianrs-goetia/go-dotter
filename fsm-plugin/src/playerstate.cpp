#include "playerstate.h"

State::StateReturn IdleState::handle_input(const godot::Ref<godot::InputEvent> &p_event)
{
    State::StateReturn ret;
    if (p_event->is_action_pressed("walk_left"))
    {
        ret.ret_enum = State::EnumStateReturn::NEW_STATE;
        ret.new_state = new WalkState;
    }
    return ret;
}

State::StateReturn WalkState::handle_input(const godot::Ref<godot::InputEvent> &p_event)
{
    State::StateReturn ret;
    if (p_event->is_action_pressed("walk_right"))
    {
        ret.ret_enum = State::EnumStateReturn::NEW_STATE;
        ret.new_state = new IdleState;
    }
    return ret;
}
