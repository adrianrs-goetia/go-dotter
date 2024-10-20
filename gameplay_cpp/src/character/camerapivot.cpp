#include <character/camerapivot.h>
#include <character/playernode.h>

#include <godot_cpp/core/math.hpp>

using namespace godot;

constexpr float X_MAX_ROTATION = 20.0f;
constexpr float X_MIN_ROTATION = -80.0f;

constexpr bool MNK_X_INVERTED = false;
constexpr bool MNK_Y_INVERTED = false;
constexpr float MNKMOTION_X_MULTIPLIER = 20.0f;
constexpr float MNKMOTION_Y_MULTIPLIER = 20.0f;

constexpr bool JOYMOTION_X_INVERTED = false;
constexpr bool JOYMOTION_Y_INVERTED = true;
constexpr float JOYMOTION_X_MULTIPLIER = 100.f;
constexpr float JOYMOTION_Y_MULTIPLIER = 100.f;

void CameraPivot::_bind_methods() {
	DEFAULT_PROPERTY(CameraPivot)
	//
}

void CameraPivot::_ready() {}

void CameraPivot::_enter_tree() { RETURN_IF_EDITOR }

void CameraPivot::_process(double delta) {}

void CameraPivot::process(StateContext& context, float delta) {
	RETURN_IF_EDITOR

	context.input->m_camera2dDir = Vector2::from_angle(-get_rotation().y - (PI_HALF));
	context.input->m_inputCameraRelative = context.input->m_inputRaw.rotated(-get_rotation().y);

	switch (context.input->mode) {
		case EInputMode::JOYPAD: {
			Vector3 current_rot = get_rotation_degrees();
			Vector2 motion = context.input->m_motion;
			current_rot.y += motion.x * JOYMOTION_X_MULTIPLIER * delta * (JOYMOTION_X_INVERTED ? 1.f : -1.f);
			current_rot.x += motion.y * JOYMOTION_Y_MULTIPLIER * delta * (JOYMOTION_Y_INVERTED ? 1.f : -1.f);
			current_rot.x = Math::clamp(current_rot.x, X_MIN_ROTATION, X_MAX_ROTATION);
			set_rotation_degrees(current_rot);
			break;
		}
		default: break;
	}
}

void CameraPivot::_physics_process(double delta) {}

void CameraPivot::processInput(StateContext& context, float delta) {
	// Want to process MnK and Joypad slightly differently
	// Joypad gets janky when only handled in input, so its updated each frame
	// Mnk gets floaty when handled each frame, so it is only updated on input events
	switch (context.input->mode) {
		case EInputMode::MOUSE_N_KEYBOARD: {
			const Vector2 motion = context.input->m_motion;
			Vector3 current_rot = get_rotation_degrees();
			current_rot.y += motion.x * MNKMOTION_X_MULTIPLIER * delta * (MNK_X_INVERTED ? 1.f : -1.f);
			current_rot.x += motion.y * MNKMOTION_Y_MULTIPLIER * delta * (MNK_Y_INVERTED ? 1.f : -1.f);
			current_rot.x = Math::clamp(current_rot.x, X_MIN_ROTATION, X_MAX_ROTATION);
			set_rotation_degrees(current_rot);
			break;
		}
		default: break;
	}
}
