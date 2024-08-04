#ifndef GD_FSM_PLUGIN_STATE_H
#define GD_FSM_PLUGIN_STATE_H

#include <assert.h>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

class FSM;

class State
{
    friend class FSM;

public:
    enum EnumStateReturn
    {
        NONE = 0,
        CONTINUE,
        NEW_STATE,
    };
    struct StateReturn
    {
        EnumStateReturn ret_enum = EnumStateReturn::NONE;
        State *new_state = nullptr;
    };

    virtual StateReturn enter_state()
    {
        printf("Entering state %s \n", get_class_name());
        return StateReturn();
    }
    virtual StateReturn process(double delta) = 0;
    virtual void exit_state() { printf("Leaving state %s \n", get_class_name()); }

    // here or otherplace?
	virtual StateReturn handle_input(const godot::Ref<godot::InputEvent> &p_event) = 0;

    virtual const char *get_class_name() = 0;

#define CLASS_NAME(CLASS)                         \
    template <typename T>                         \
    const char *_get_class_name()                 \
    {                                             \
        return typeid(T).name();                  \
    }                                             \
    virtual const char *get_class_name() override \
    {                                             \
        return _get_class_name<CLASS>();          \
    }
};

class FSM : public godot::Node
{
    GDCLASS(FSM, godot::Node)

protected:
    State *_current_state = nullptr;

public:
    static void _bind_methods(){}
    virtual void _enter_tree() override;
	virtual void _exit_tree() override;
    virtual void _process(double delta);
	virtual void _unhandled_input(const godot::Ref<godot::InputEvent> &p_event);

protected:
    void process_state(State::StateReturn state_return);
};

#endif // GD_FSM_PLUGIN_STATE_H