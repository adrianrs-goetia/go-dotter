#include <mainnode.h>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;

void MainNode::_bind_methods() { DEFAULT_PROPERTY(MainNode) }

void MainNode::_unhandled_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR
	if (p_event->is_action_pressed(InputMap::pause_menu)) {
		if (SceneTree* tree = get_tree()) {
			tree->quit(0);
		}
	}
	else if (p_event->is_action_pressed(InputMap::toggle_screen_mode)) {
		Log(ELog::DEBUG, "Toggle primary screen mode");
		DisplayServer* ds = DisplayServer::get_singleton();
		int prime_screen = ds->get_primary_screen();
		DisplayServer::WindowMode mode = ds->window_get_mode(prime_screen);

		if (mode == DisplayServer::WindowMode::WINDOW_MODE_EXCLUSIVE_FULLSCREEN) {
			ds->window_set_mode(DisplayServer::WindowMode::WINDOW_MODE_WINDOWED, prime_screen);
			ds->window_set_size(Vector2i(1280, 720), prime_screen);
			Vector2i size = ds->screen_get_size();
			ds->window_set_position(size / 4, prime_screen);
		}
		else {
			ds->window_set_mode(DisplayServer::WindowMode::WINDOW_MODE_EXCLUSIVE_FULLSCREEN);
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
