#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

class PauseMenu : public godot::Control {
	GDTYPE(PauseMenu, godot::Control)

	bool active{ false };

public:
	static void _bind_methods() {}

	void _enter_tree() {
		RETURN_IF_EDITOR(void())
		deactivate();
	}

	void _notification(int what) {
		/**
		 * Pause signals can arrive from anywhere
		 * But currently only from InputManager and Level select buttons
		 *
		 * 'Paused' as in node is paused from processing, not game is paused
		 * Node is set to only process when tree->is_paused is true
		 */
		switch (what) {
			case NOTIFICATION_PAUSED:
				deactivate();
				break;
			case NOTIFICATION_UNPAUSED:
				activate();
				break;
			default:
				break;
		}
	}

	void toggle() {
		active ? deactivate() : activate();
	}

	void activate() {
		LOG(DEBUG, "PAUSE_GAME")
		active = true;
		set_visible(true);
		set_z_index(godot::RenderingServer::CANVAS_ITEM_Z_MAX); // Always draw this on top of everything else

		// have to set mouse mode capture here.
		// @todo: move this to InputManager, mouse should not be visible while using gamepad
		godot::Input::get_singleton()->set_mouse_mode(godot::Input::MOUSE_MODE_VISIBLE);
	}

	void deactivate() {
		LOG(DEBUG, "UNPAUSE_GAME")
		active = false;
		set_visible(false);

		godot::Input::get_singleton()->set_mouse_mode(godot::Input::MOUSE_MODE_CAPTURED);
	}
};
