#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>

namespace InputString {

constexpr const char* moveLeft = "move_left";
constexpr const char* moveRight = "move_right";
constexpr const char* moveForward = "move_forward";
constexpr const char* moveBackward = "move_backward";

constexpr const char* jump = "jump";
constexpr const char* attack = "attack";
constexpr const char* parry = "parry";
constexpr const char* grapplehook = "grapplehook";

constexpr const char* cameraLeft = "camera_left";
constexpr const char* cameraRight = "camera_right";
constexpr const char* cameraUp = "camera_up";
constexpr const char* cameraDown = "camera_down";

constexpr const char* pauseMenu = "pause_menu";
constexpr const char* toggleScreenMode = "toggle_screen_mode";
constexpr const char* toggleApplicationMouseLock = "toggle_application_mouse_lock";
constexpr const char* restart = "restart";

// Some Built-in actions
constexpr const char* uiUp = "ui_up";
constexpr const char* uiDown = "ui_down";

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

class InputAction {
	bool m_consumed = false;
	EInputAction m_action = EInputAction::NONE;
	EInputActionType m_type = EInputActionType::NONE;
	Timestamp m_timestamp;

public:
	InputAction() = default;
	InputAction(EInputAction action, EInputActionType type)
		: m_action(action)
		, m_type(type) {}

	bool isActionPressed(EInputAction action, bool consume = true) {
		if (!m_consumed && m_action == action) {
			m_consumed = consume;
			return m_type == EInputActionType::PRESSED;
		}
		return false;
	}
	bool isActionReleased(EInputAction action) { return m_action == action && m_type == EInputActionType::RELEASED; }
	bool isActionHeld(EInputAction action) { return m_action == action && m_type == EInputActionType::HELD; }
	bool isActionDown(EInputAction action) {
		return m_action == action && (m_type == EInputActionType::HELD || m_type == EInputActionType::PRESSED);
	}
	bool receivedInputWithinTimeframe(float timeframe_seconds) {
		return m_timestamp.timestampWithinTimeframe(timeframe_seconds);
	}
	bool isConsumed() const { return m_consumed; }
};

/**
 * Is automatically added to root of the running scene
 */
class InputManager : public godot::Node {
	GDCLASS(InputManager, godot::Node)

	static constexpr const char* g_inputManagerNodePath = "/root/InputManager";

public:
	EInputMode mode;

	/**
	 * Raw input along XY axis
	 */
	godot::Vector2 m_inputRaw;
	/**
	 * Input in 3D space relative to camera XZ
	 */
	godot::Vector2 m_inputCameraRelative;
	godot::Vector2 m_motion; // Mouse motion
	godot::Vector2 m_camera2dDir;

	std::list<InputAction> m_inputActions;

	struct AdditionalStates {
		bool applicationMouseLock{ true };
	} m_additionalStates;

private:
	void exitGame();

public:
	static void _bind_methods();

	void _notification(int what);
	void _enter_tree() override;
	void _physics_process(double delta);
	void _input(const godot::Ref<godot::InputEvent>& p_event);
	void _unhandled_input(const godot::Ref<godot::InputEvent>& p_event);

	bool isActionPressed(EInputAction action, float timeframe = -1.0f);

	/**
	 * Reference node needed as we don't have a handle on the scene itself otherwise
	 */
	static InputManager* get(const Node& referenceNode);

	godot::Vector3 getCamera3dDir() const;
	godot::Vector3 getInputRaw3d() const;
	godot::Vector3 getInputRelative3d(float y = 0.f) const;
};