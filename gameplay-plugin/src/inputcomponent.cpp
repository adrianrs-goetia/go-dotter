#include <components/inputcomponent.h>

#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

void InputComponent::_bind_methods() { DEFAULT_PROPERTY(InputComponent) }

void InputComponent::_process(float delta) {
	godot::Input* input = Input::get_singleton();
	// Input movement direction (wasd or leftstick equivalent)
	input_raw = input->get_vector(
			InputMap::move_left, InputMap::move_right, InputMap::move_forward, InputMap::move_backward, 0.05);
}

void InputComponent::_input(const Ref<InputEvent>& p_event) {
	godot::Input* input = Input::get_singleton();

	ASSERT(DisplayServer::get_singleton() != nullptr, "");
	if (DisplayServer::get_singleton()->window_is_focused()) input->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

	// Camera motion (mouse or rightstick equivalent)
	if (auto* p_mousemotion = cast_to<InputEventMouseMotion>(*p_event)) {
		mode = EInputMode::MOUSE_N_KEYBOARD;
		motion = p_mousemotion->get_relative();
	}
	else if (auto* p_joypadmotion = cast_to<InputEventJoypadMotion>(*p_event)) {
		mode = EInputMode::JOYPAD;
		motion = input->get_vector(
				InputMap::camera_left, InputMap::camera_right, InputMap::camera_down, InputMap::camera_up, 0.05);
	}
	else { mode = EInputMode::NONE; }

// Input actions
if (p_event->is_action_pressed(InputMap::jump)) {
		input_action = InputAction{ EInputAction::JUMP, EInputActionType::PRESSED };
	}
	else {
		last_valid_input_action = input_action;
		input_action = InputAction{ EInputAction::NONE, EInputActionType::NONE };
	}
}

void InputComponent::_unhandled_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR
	if (p_event->is_action_pressed(InputMap::pause_menu)) {
		if (SceneTree* tree = get_tree()) {
			LOG(DEBUG, "tree->quit(0)")
			tree->quit(0);
		}
	}
	else if (p_event->is_action_pressed(InputMap::toggle_screen_mode)) {
		LOG(DEBUG, "Toggle primary screen mode");
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
	else if (p_event->is_action_pressed(InputMap::restart)) {
		if (SceneTree* tree = get_tree()) {
			LOG(DEBUG, "Reloading current scene")
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

InputComponent* InputComponent::getinput(const Node* referencenode) {
	if (auto* input = referencenode->get_node<InputComponent>("/root/Inputcomponent")) { return input; }
	return nullptr;
}

Vector3 InputComponent::get_camera3ddir() const { return Vector3(camera2ddir.x, 0, camera2ddir.y).normalized(); }

Vector3 InputComponent::get_input_relative_3d() const {
	return Vector3(input_relative.x, 0, input_relative.y).normalized();
}
