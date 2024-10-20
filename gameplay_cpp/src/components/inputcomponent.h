#pragma once

#include <core/core.h>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>

using namespace godot;

namespace InputString {

	constexpr const char* move_left = "move_left";
	constexpr const char* move_right = "move_right";
	constexpr const char* move_forward = "move_forward";
	constexpr const char* move_backward = "move_backward";

	constexpr const char* jump = "jump";
	constexpr const char* attack = "attack";
	constexpr const char* parry = "parry";
	constexpr const char* grapplehook = "grapplehook";

	constexpr const char* camera_left = "camera_left";
	constexpr const char* camera_right = "camera_right";
	constexpr const char* camera_up = "camera_up";
	constexpr const char* camera_down = "camera_down";

	constexpr const char* pause_menu = "pause_menu";
	constexpr const char* toggle_screen_mode = "toggle_screen_mode";
	constexpr const char* toggle_application_mouse_lock = "toggle_application_mouse_lock";
	constexpr const char* restart = "restart";

	// Some Built-in actions
	constexpr const char* ui_up = "ui_up";
	constexpr const char* ui_down = "ui_down";

} //namespace InputString

enum class EInputMode : uint8_t {
	NONE = 0,
	MOUSE_N_KEYBOARD = 1,
	JOYPAD = 2,
};

enum class EInputAction : uint8_t {
	NONE = 0,
	// Game action
	// MOVE_LEFT,
	// MOVE_RIGHT,
	JUMP,
	GRAPPLE,
	PARRY,
	ATTACK,
	PAUSE_MENU,

	// Some Built-in Actions
	// UI_ACCEPT,
	// UI_SELECT,
	// UI_CANCEL,
	// UI_FOCUS_NEXT,
	// UI_FOCUS_PREV,
	// UI_LEFT,
	// UI_RIGHT,
	// UI_UP,
	// UI_DOWN,
	// UI_MENU,
};

enum class EInputActionType : uint8_t {
	NONE = 0,
	PRESSED,
	RELEASED,
	HELD,
};

// Input action passed down to the players fsm->current_state
// size == 10 bytes + potential padding
struct InputAction {
	InputAction() = default;
	InputAction(EInputAction action, EInputActionType type)
		: _action(action)
		, _type(type) {}
	bool _consumed = false;
	EInputAction _action = EInputAction::NONE;
	EInputActionType _type = EInputActionType::NONE;
	Timestamp _timestamp;

	bool is_action_pressed(EInputAction action, bool consume = true) {
		if (!_consumed && _action == action) {
			_consumed = consume;
			return _type == EInputActionType::PRESSED;
		}
		return false;
	}
	bool is_action_released(EInputAction action) { return _action == action && _type == EInputActionType::RELEASED; }
	bool is_action_held(EInputAction action) { return _action == action && _type == EInputActionType::HELD; }
	bool is_action_down(EInputAction action) {
		return _action == action && (_type == EInputActionType::HELD || _type == EInputActionType::PRESSED);
	}
	bool received_input_within_timeframe(float timeframe_seconds) {
		return _timestamp.timestampWithinTimeframe(timeframe_seconds);
	}
	bool is_consumed() const { return _consumed; }
};

/**
 * Is automatically added to root of the running scene
 */
class InputComponent : public Node {
	GDCLASS(InputComponent, Node)

public:
	EInputMode mode;

	/**
	 * Raw input along x/y axis
	 */
	Vector2 input_raw;
	/**
	 * Input in 3D space relative to camera
	 */
	Vector2 m_input_relative;
	Vector2 m_motion;
	Vector2 m_camera2ddir;

	std::list<InputAction> input_actions;

	struct AdditionalStates {
		bool application_mouse_lock{ true };
	} m_additionalStates;

private:
	void exit_game();

public:
	GETNAME(InputComponent)
	static void _bind_methods();

	void _notification(int what);
	void _physics_process(double delta);
	void _input(const Ref<InputEvent>& p_event);
	void _unhandled_input(const Ref<InputEvent>& p_event);

	bool is_action_pressed(EInputAction action, float timeframe = -1.0f);

	static InputComponent* getinput(const Node* referencenode);

	Vector3 get_camera3ddir() const;
	Vector3 get_input_raw_3d() const;
	Vector3 get_input_relative_3d(float y = 0.f) const;
};