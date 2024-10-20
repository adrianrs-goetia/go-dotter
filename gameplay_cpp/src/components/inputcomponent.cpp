#include <components/inputcomponent.h>

#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

static constexpr float MAX_INPUT_LIFETIME = 0.1f;

void InputComponent::exitGame() {
	if (SceneTree* tree = get_tree()) {
		LOG(INFO, "tree->quit(0)")
		tree->quit(0);
	}
}

void InputComponent::_bind_methods() {
	DEFAULT_PROPERTY(InputComponent)

	godot::ClassDB::bind_method(godot::D_METHOD("exitGame"), &InputComponent::exitGame);
}

void InputComponent::_physics_process(double delta) {
	RETURN_IF_EDITOR
	godot::Input* input = Input::get_singleton();

	// Input movement direction (wasd or leftstick equivalent)
	m_inputRaw = input->get_vector(
			InputString::moveLeft, InputString::moveRight, InputString::moveForward, InputString::moveBackward, 0.05);

	// Input actions
	if (input->is_action_just_pressed(InputString::jump)) {
		m_inputActions.emplace_back(InputAction{ EInputAction::JUMP, EInputActionType::PRESSED });
	}
	if (input->is_action_just_pressed(InputString::grapplehook)) {
		m_inputActions.emplace_back(InputAction{ EInputAction::GRAPPLE, EInputActionType::PRESSED });
	}
	if (input->is_action_just_pressed(InputString::parry)) {
		m_inputActions.emplace_back(InputAction{ EInputAction::PARRY, EInputActionType::PRESSED });
	}

	// Clear inputs
	for (auto it = m_inputActions.begin(); it != m_inputActions.end();) {
		if (it->isConsumed() || !it->receivedInputWithinTimeframe(MAX_INPUT_LIFETIME)) {
			it = m_inputActions.erase(it);
			continue;
		}
		++it;
	}
}

void InputComponent::_notification(int what) {
	// LOG(INFO, "Notification", (int64_t)what)
	switch (what) {
		case NOTIFICATION_WM_CLOSE_REQUEST: {
			LOG(INFO, "InputComponent WM_CLOSE_REQUEST")
			// exitGame();
			break;
		}

		default: break;
	}
}

void InputComponent::_input(const Ref<InputEvent>& p_event) {
	godot::Input* input = Input::get_singleton();

	ASSERT(DisplayServer::get_singleton() != nullptr, "");
	if (DisplayServer::get_singleton()->window_is_focused()) {
		m_additionalStates.applicationMouseLock ? input->set_mouse_mode(Input::MOUSE_MODE_CAPTURED)
												: input->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
	}

	// Camera motion (mouse or rightstick equivalent)
	if (auto* p_mousemotion = cast_to<InputEventMouseMotion>(*p_event)) {
		mode = EInputMode::MOUSE_N_KEYBOARD;
		m_motion = p_mousemotion->get_relative();
	}
	else if (auto* p_joypadmotion = cast_to<InputEventJoypadMotion>(*p_event)) {
		mode = EInputMode::JOYPAD;
		m_motion = input->get_vector(InputString::cameraLeft, InputString::cameraRight, InputString::cameraDown,
				InputString::cameraUp, 0.05);
	}
	else { mode = EInputMode::NONE; }
}

void InputComponent::_unhandled_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR
	if (p_event->is_action_pressed(InputString::pauseMenu)) {
		LOG(INFO, "tree notification WM_CLOSE_REQUEST")
		exitGame();
		return;
	}
	else if (p_event->is_action_pressed(InputString::toggleScreenMode)) {
		LOG(INFO, "Toggle primary screen mode");
		DisplayServer* ds = DisplayServer::get_singleton();
		int prime_screen = ds->get_primary_screen();
		DisplayServer::WindowMode mode = ds->window_get_mode(prime_screen);

		if (mode == DisplayServer::WindowMode::WINDOW_MODE_EXCLUSIVE_FULLSCREEN) {
			ds->window_set_mode(DisplayServer::WindowMode::WINDOW_MODE_WINDOWED, prime_screen);
			ds->window_set_size(Vector2i(1280, 720), prime_screen);
			Vector2i size = ds->screen_get_size();
			ds->window_set_position(size / 4, prime_screen);
		}
		else { ds->window_set_mode(DisplayServer::WindowMode::WINDOW_MODE_EXCLUSIVE_FULLSCREEN); }
	}
	else if (p_event->is_action_pressed(InputString::toggleApplicationMouseLock)) {
		m_additionalStates.applicationMouseLock = !m_additionalStates.applicationMouseLock;
	}
	else if (p_event->is_action_pressed(InputString::restart)) {
		if (SceneTree* tree = get_tree()) {
			LOG(INFO, "Reloading current scene")
			tree->reload_current_scene();
		}
	}
	if (auto* keyevent = cast_to<InputEventKey>(*p_event)) {
		switch (keyevent->get_keycode()) {
			case KEY_7: return set_log_level(ELog::DEBUG);
			case KEY_8: return set_log_level(ELog::INFO);
			case KEY_9: return set_log_level(ELog::WARN);
			case KEY_0: return set_log_level(ELog::ERROR);
			default: break;
		}
	}
}

bool InputComponent::isActionPressed(EInputAction action, float timeframe) {
	if (timeframe > 0.f) {
		for (auto it = m_inputActions.begin(); it != m_inputActions.end(); it++) {
			if (it->isActionPressed(action) && it->receivedInputWithinTimeframe(timeframe)) {
				m_inputActions.erase(it);
				return true;
			}
		}
	}
	if (!m_inputActions.empty()) { return m_inputActions.begin()->isActionPressed(action); }
	return false;
}

InputComponent* InputComponent::getinput(const Node* referencenode) {
	if (auto* input = referencenode->get_node<InputComponent>("/root/Inputcomponent")) { return input; }
	return nullptr;
}

Vector3 InputComponent::getCamera3dDir() const { return Vector3(m_camera2dDir.x, 0, m_camera2dDir.y).normalized(); }

Vector3 InputComponent::getInputRaw3d() const { return Vector3(m_inputRaw.x, 0, m_inputRaw.y).normalized(); }

Vector3 InputComponent::getInputRelative3d(float y) const {
	return Vector3(m_inputCameraRelative.x, y, m_inputCameraRelative.y).normalized();
}
