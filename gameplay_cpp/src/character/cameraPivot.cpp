#include <character/cameraPivot.h>
#include <character/playerNode.h>

#include <godot_cpp/core/math.hpp>

using namespace godot;

void CameraPivot::_bind_methods() {}

void CameraPivot::process(const fsm::player::Context& context, float delta) {
	RETURN_IF_EDITOR(void())

	context.input->m_camera2dDir = Vector2::from_angle(-get_rotation().y - (PI_HALF));
	context.input->m_inputCameraRelative = context.input->m_inputRaw.rotated(-get_rotation().y);

	switch (context.input->m_mode) {
		case EInputMode::KEYBOARD_ONLY: {
			_rotation(context,
				param.keyboardOnly.xMultiplier() * (param.keyboardOnly.xInverted() ? 1.f : -1.f),
				param.keyboardOnly.yMultiplier() * (param.keyboardOnly.yInverted() ? 1.f : -1.f),
				delta);
			break;
		}
		case EInputMode::JOYPAD: {
			_rotation(context,
				param.joypad.xMultiplier() * (param.joypad.xInverted() ? 1.f : -1.f),
				param.joypad.yMultiplier() * (param.joypad.yInverted() ? 1.f : -1.f),
				delta);
			break;
		}
		default:
			break;
	}
}

void CameraPivot::_physics_process(double delta) {}

void CameraPivot::processInput(const fsm::player::Context& context, float delta) {
	// Want to process MnK and Joypad slightly differently
	// Joypad gets janky when only handled in input, so its updated each frame
	// Mnk gets floaty when handled each frame, so it is only updated on input events
	switch (context.input->m_mode) {
		case EInputMode::MOUSE_N_KEYBOARD: {
			_rotation(context,
				param.mnk.xMultiplier() * (param.mnk.xInverted() ? 1.f : -1.f),
				param.mnk.yMultiplier() * (param.mnk.yInverted() ? 1.f : -1.f),
				delta);
			break;
		}
		default:
			break;
	}
}

void CameraPivot::_rotation(const fsm::player::Context& context, float xMulti, float yMulti, float delta) {
	Vector3 current_rot = get_rotation_degrees();
	Vector2 motion = context.input->m_motion;
	current_rot.y += motion.x * xMulti * delta;
	current_rot.x += motion.y * yMulti * delta;
	current_rot.x = Math::clamp(current_rot.x, param.xMinRotation(), param.xMaxRotation());
	set_rotation_degrees(current_rot);
}
