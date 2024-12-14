#include <character/cameraPivot.h>
#include <character/playerNode.h>

#include <godot_cpp/core/math.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "camera"

using namespace godot;

void CameraPivot::_bind_methods() {}

void CameraPivot::process(StateContext& context, float delta) {
	RETURN_IF_EDITOR(void())

	context.input->m_camera2dDir = Vector2::from_angle(-get_rotation().y - (PI_HALF));
	context.input->m_inputCameraRelative = context.input->m_inputRaw.rotated(-get_rotation().y);

	switch (context.input->m_mode) {
		case EInputMode::KEYBOARD_ONLY: {
			_rotation(context,
					GETPARAM_F("keyboardOnly", "xMultiplier") * (GETPARAM_B("keyboardOnly", "xInverted") ? 1.f : -1.f),
					GETPARAM_F("keyboardOnly", "yMultiplier") * (GETPARAM_B("keyboardOnly", "yInverted") ? 1.f : -1.f),
					delta);
			break;
		}
		case EInputMode::JOYPAD: {
			_rotation(context, GETPARAM_F("joypad", "xMultiplier") * (GETPARAM_B("joypad", "xInverted") ? 1.f : -1.f),
					GETPARAM_F("joypad", "yMultiplier") * (GETPARAM_B("joypad", "yInverted") ? 1.f : -1.f), delta);
			break;
		}
		default:
			break;
	}
}

void CameraPivot::_physics_process(double delta) {}

void CameraPivot::processInput(StateContext& context, float delta) {
	// Want to process MnK and Joypad slightly differently
	// Joypad gets janky when only handled in input, so its updated each frame
	// Mnk gets floaty when handled each frame, so it is only updated on input events
	switch (context.input->m_mode) {
		case EInputMode::MOUSE_N_KEYBOARD: {
			_rotation(context, GETPARAM_F("mnk", "xMultiplier") * (GETPARAM_B("mnk", "xInverted") ? 1.f : -1.f),
					GETPARAM_F("mnk", "yMultiplier") * (GETPARAM_B("mnk", "yInverted") ? 1.f : -1.f), delta);
			break;
		}
		default:
			break;
	}
}

void CameraPivot::_rotation(StateContext& context, float xMulti, float yMulti, float delta) {
	Vector3 current_rot = get_rotation_degrees();
	Vector2 motion = context.input->m_motion;
	current_rot.y += motion.x * xMulti * delta;
	current_rot.x += motion.y * yMulti * delta;
	current_rot.x = Math::clamp(current_rot.x, GETPARAM_F("xMinRotation"), GETPARAM_F("xMaxRotation"));
	set_rotation_degrees(current_rot);
}
