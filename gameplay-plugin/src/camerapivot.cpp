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
constexpr bool JOYMOTION_Y_INVERTED = true;
constexpr float JOYMOTION_X_MULTIPLIER = 100.f;
constexpr float JOYMOTION_Y_MULTIPLIER = 100.f;

void CameraPivot::_bind_methods() { DEFAULT_PROPERTY(CameraPivot) }

void CameraPivot::_enter_tree() { RETURN_IF_EDITOR }

void CameraPivot::process(StateContext* context, float delta) {
	RETURN_IF_EDITOR
	switch (context->input.mode) {
		case EInputMode::JOYPAD: {
			Vector3 current_rot = get_rotation_degrees();
			current_rot.y += context->input.motion.x;
			current_rot.x += context->input.motion.y;
			current_rot.x = Math::clamp(current_rot.x, X_MIN_ROTATION, X_MAX_ROTATION);
			set_rotation_degrees(current_rot);
			break;
		}
		default: break;
	}
}

void CameraPivot::_physics_process(float delta) {}

void CameraPivot::process_input(
		StateContext* context, godot::Input* input, const Ref<InputEvent>& p_event, float delta) {
	input->set_mouse_mode(Input::MOUSE_MODE_HIDDEN);
	context->input.camera2ddir = Vector2::from_angle(-get_rotation().y);

	if (auto* p_mousemotion = cast_to<InputEventMouseMotion>(*p_event)) {
		context->input.mode = EInputMode::MOUSE_N_KEYBOARD;
		Vector2 motion = p_mousemotion->get_velocity();
		Log(ELog::INFO, "motion", motion);

		Vector3 current_rot = get_rotation_degrees();
		current_rot.y += motion.x * MNKMOTION_X_MULTIPLIER * delta * (MNK_X_INVERTED ? 1.f : -1.f);
		current_rot.x += motion.y * MNKMOTION_Y_MULTIPLIER * delta * (MNK_Y_INVERTED ? 1.f : -1.f);
		current_rot.x = Math::clamp(current_rot.x, X_MIN_ROTATION, X_MAX_ROTATION);
		set_rotation_degrees(current_rot);
	}
	else if (auto* p_joypadmotion = cast_to<InputEventJoypadMotion>(*p_event)) {
		context->input.mode = EInputMode::JOYPAD;
		context->input.motion = input->get_vector(
				InputMap::camera_left, InputMap::camera_right, InputMap::camera_down, InputMap::camera_up, 0.05);
		context->input.motion.x *= JOYMOTION_X_MULTIPLIER * delta * (JOYMOTION_X_INVERTED ? 1.f : -1.f);
		context->input.motion.y *= JOYMOTION_Y_MULTIPLIER * delta * (JOYMOTION_Y_INVERTED ? 1.f : -1.f);
	}
	else {
		context->input.mode = EInputMode::NONE;
	}
}
