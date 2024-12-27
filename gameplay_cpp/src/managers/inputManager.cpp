#include "inputManager.h"

#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/editor_command_palette.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include <applicationparams.hpp>

using namespace godot;

static constexpr float MAX_INPUT_LIFETIME = 0.1f;

void InputManager::exitGame() {
	if (SceneTree* tree = get_tree()) {
		LOG(INFO, "tree->quit(0)")
		tree->quit(0);
	}
}

InputManager::InputManager() {
	set_process(true);
	set_process_internal(true); // Is autoloaded into scene, but this *should* ensure it always runs during editor
}

void InputManager::_bind_methods() {
	ClassDB::bind_method(godot::D_METHOD("exitGame"), &InputManager::exitGame);
}

void InputManager::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())
	Input* input = Input::get_singleton();

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
	if (input->is_action_just_pressed(InputString::attack)) {
		m_inputActions.emplace_back(InputAction{ EInputAction::ATTACK, EInputActionType::PRESSED });
	}
	if (input->is_action_pressed(InputString::sprint)) {
		m_inputActions.emplace_back(InputAction{ EInputAction::SPRINT, EInputActionType::HELD });
	}

	// Motion vector for keyboard only
	if (m_mode == EInputMode::KEYBOARD_ONLY) {
		Vector2 motion;
		if (input->is_action_pressed(InputString::cameraLeft)) {
			motion.x -= 1.f;
		}
		if (input->is_action_pressed(InputString::cameraRight)) {
			motion.x += 1.f;
		}
		if (input->is_action_pressed(InputString::cameraDown)) {
			motion.y -= 1.f;
		}
		if (input->is_action_pressed(InputString::cameraUp)) {
			motion.y += 1.f;
		}
		m_motion = motion;
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

void InputManager::_notification(int what) {
	// LOG(INFO, "Notification", (int64_t)what)
	switch (what) {
		case NOTIFICATION_WM_CLOSE_REQUEST: {
			LOG(INFO, "InputManager WM_CLOSE_REQUEST")
			// exitGame();
			break;
		}

		default:
			break;
	}
}


void InputManager::_enter_tree() {
	set_name(get_class());

	RETURN_IF_EDITOR(void())
}

void InputManager::_input(const Ref<InputEvent>& p_event) {
	godot::Input* input = Input::get_singleton();

	ASSERTNN(DisplayServer::get_singleton())
	if (DisplayServer::get_singleton()->window_is_focused()) {
		const bool mouselock = ApplicationParam::Input::mouselock();
		mouselock ? input->set_mouse_mode(Input::MOUSE_MODE_CAPTURED)
				  : input->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
	}

	const auto overwriteMode = ApplicationParam::Input::overwriteMode();
	if (overwriteMode != static_cast<int>(EInputMode::NONE)) {
		m_mode = static_cast<EInputMode>(overwriteMode);
		switch (m_mode) {
			case EInputMode::MOUSE_N_KEYBOARD: {
				if (auto* p_mousemotion = cast_to<InputEventMouseMotion>(*p_event)) {
					m_motion = p_mousemotion->get_relative();
				}
				break;
			}
			case EInputMode::JOYPAD: {
				if (auto* p_joypadmotion = cast_to<InputEventJoypadMotion>(*p_event)) {
					m_motion = input->get_vector(InputString::cameraLeft,
						InputString::cameraRight,
						InputString::cameraDown,
						InputString::cameraUp,
						0.05);
				}
				break;
			}
			default:
				break;
		}
	}
	else { // Autodetect mode
		if (auto* p_mousemotion = cast_to<InputEventMouseMotion>(*p_event)) {
			m_mode = EInputMode::MOUSE_N_KEYBOARD;
			m_motion = p_mousemotion->get_relative();
		}
		else if (auto* p_joypadmotion = cast_to<InputEventJoypadMotion>(*p_event)) {
			m_mode = EInputMode::JOYPAD;
			m_motion = input->get_vector(InputString::cameraLeft,
				InputString::cameraRight,
				InputString::cameraDown,
				InputString::cameraUp,
				0.05);
		}
	}
}

void InputManager::_unhandled_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR(void())
	if (p_event->is_action_pressed(InputString::pauseMenu)) {
		LOG(INFO, "tree notification WM_CLOSE_REQUEST")
		exitGame();
		return;
	}
	else if (p_event->is_action_pressed(InputString::toggleScreenMode)) {
		LOG(INFO, "Toggle primary screen m_mode");
		DisplayServer* ds = DisplayServer::get_singleton();
		int prime_screen = ds->get_primary_screen();
		DisplayServer::WindowMode m_mode = ds->window_get_mode(prime_screen);

		if (m_mode == DisplayServer::WindowMode::WINDOW_MODE_EXCLUSIVE_FULLSCREEN) {
			ds->window_set_mode(DisplayServer::WindowMode::WINDOW_MODE_WINDOWED, prime_screen);
			ds->window_set_size(Vector2i(1280, 720), prime_screen);
			Vector2i size = ds->screen_get_size();
			ds->window_set_position(size / 4, prime_screen);
		}
		else {
			ds->window_set_mode(DisplayServer::WindowMode::WINDOW_MODE_EXCLUSIVE_FULLSCREEN);
		}
	}
	else if (p_event->is_action_pressed(InputString::restart)) {
		if (SceneTree* tree = get_tree()) {
			LOG(INFO, "Reloading current scene")
			if (ApplicationParam::hardResetTreeOnRestart()) {
				tree->reload_current_scene();
			}
			else {
				tree->get_current_scene()->propagate_notification(ENotifications::SOFT_RESTART);
			}
		}
	}
	if (auto* keyevent = cast_to<InputEventKey>(*p_event)) {
		switch (keyevent->get_keycode()) {
			case KEY_7:
				return setLogLevel(ELog::DEBUG);
			case KEY_8:
				return setLogLevel(ELog::INFO);
			case KEY_9:
				return setLogLevel(ELog::WARN);
			case KEY_0:
				return setLogLevel(ELog::ERROR);
			default:
				break;
		}
	}
}

bool InputManager::isActionPressed(EInputAction action, float timeframe) {
	if (timeframe > 0.f) {
		for (auto it = m_inputActions.begin(); it != m_inputActions.end(); it++) {
			if (it->isActionPressed(action) && it->receivedInputWithinTimeframe(timeframe)) {
				m_inputActions.erase(it);
				return true;
			}
		}
	}
	if (!m_inputActions.empty()) {
		return m_inputActions.begin()->isActionPressed(action);
	}
	return false;
}

bool InputManager::isActionHeld(EInputAction action) {
	for (auto it = m_inputActions.begin(); it != m_inputActions.end(); it++) {
		if (it->isActionHeld(action)) {
			LOG(DEBUG, "STINPIN'!!!");
			return true;
		}
	}
	if (!m_inputActions.empty()) {
		return m_inputActions.begin()->isActionHeld(action);
	}
	return false;
}

InputManager* InputManager::get(const Node& referenceNode) {
	if (auto* input = referenceNode.get_node<InputManager>(g_inputManagerNodePath)) {
		return input;
	}
	return nullptr;
}

Vector3 InputManager::getCamera3dDir() const {
	return Vector3(m_camera2dDir.x, 0, m_camera2dDir.y).normalized();
}

Vector3 InputManager::getInputRaw3d() const {
	return Vector3(m_inputRaw.x, 0, m_inputRaw.y).normalized();
}

Vector3 InputManager::getInputRelative3d(float y) const {
	return Vector3(m_inputCameraRelative.x, y, m_inputCameraRelative.y).normalized();
}

bool InputManager::isInputActive() const {
	return m_inputRaw.length_squared() > 0.2f;
}
