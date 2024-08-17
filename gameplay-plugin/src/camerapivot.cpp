#include <character/camerapivot.h>
#include <character/playernode.h>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;

constexpr float X_MAX_ROTATION = 20.0f;
constexpr float X_MIN_ROTATION = -80.0f;

constexpr bool MNK_X_INVERTED = false;
constexpr bool MNK_Y_INVERTED = false;
constexpr float MNKMOTION_X_MULTIPLIER = 0.6f;
constexpr float MNKMOTION_Y_MULTIPLIER = 0.6f;

constexpr bool JOYMOTION_X_INVERTED = false;
constexpr bool JOYMOTION_Y_INVERTED = false;
constexpr float JOYMOTION_X_MULTIPLIER = 100.f;
constexpr float JOYMOTION_Y_MULTIPLIER = 100.f;

void CameraPivot::_bind_methods() { DEFAULT_PROPERTY(CameraPivot) }

void CameraPivot::_enter_tree() {
	RETURN_IF_EDITOR
	m_playernode = get_node<PlayerNode>(NodePath(NodePaths::player));
	if (!m_playernode) {
		printf("%s camera pivot failed to fetch playernode \n", __FUNCTION__);
	}
}

void CameraPivot::_process(float delta) {
	RETURN_IF_EDITOR
	switch (m_inputmode) {
		case EInputMode::JOYPAD: {
			Vector3 current_rot = get_rotation_degrees();
			current_rot.y += m_inputmotion.x;
			current_rot.x += m_inputmotion.y;
			current_rot.x = Math::clamp(current_rot.x, X_MIN_ROTATION, X_MAX_ROTATION);
			set_rotation_degrees(current_rot);
			break;
		}
		default:
			break;
	}
}

void CameraPivot::_physics_process(float delta) {}

void CameraPivot::_input(const Ref<InputEvent>& p_event) {
	const float delta = get_process_delta_time();
	Input* input = Input::get_singleton();
	input->set_mouse_mode(Input::MOUSE_MODE_HIDDEN);

	if (auto* p_mousemotion = cast_to<InputEventMouseMotion>(*p_event)) {
		m_inputmode = EInputMode::MOUSE_N_KEYBOARD;
		Vector2 motion = p_mousemotion->get_velocity();

		Vector3 current_rot = get_rotation_degrees();
		current_rot.y += motion.x * MNKMOTION_X_MULTIPLIER * delta * (MNK_X_INVERTED ? 1.f : -1.f);
		current_rot.x += motion.y * MNKMOTION_Y_MULTIPLIER * delta * (MNK_Y_INVERTED ? 1.f : -1.f);
		current_rot.x = Math::clamp(current_rot.x, X_MIN_ROTATION, X_MAX_ROTATION);
		set_rotation_degrees(current_rot);
	}
	else if (auto* p_joypadmotion = cast_to<InputEventJoypadMotion>(*p_event)) {
		m_inputmode = EInputMode::JOYPAD;
		m_inputmotion = input->get_vector(
				InputMap::camera_left, InputMap::camera_right, InputMap::camera_down, InputMap::camera_up, 0.05);
		m_inputmotion.x *= JOYMOTION_X_MULTIPLIER * delta * (JOYMOTION_X_INVERTED ? 1.f : -1.f);
		m_inputmotion.y *= JOYMOTION_Y_MULTIPLIER * delta * (JOYMOTION_Y_INVERTED ? 1.f : -1.f);
	}
	else {
		m_inputmode = EInputMode::NONE;
	}
}
