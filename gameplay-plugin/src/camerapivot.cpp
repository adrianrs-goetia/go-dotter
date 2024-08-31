#include <character/camerapivot.h>
#include <character/playernode.h>

#include <godot_cpp/core/math.hpp>

#include <debugdraw3d/api.h>

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

	// ClassDB::bind_method(D_METHOD("set_length_impl", "length"), &CameraPivot::set_length_impl);
	// ClassDB::bind_method(D_METHOD("get_length_impl"), &CameraPivot::get_length_impl);
	// ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spring_length_impl", PROPERTY_HINT_NONE, "suffix:m"),
	// "set_length_impl", 		"get_length_impl");
}

void CameraPivot::_ready() {}

void CameraPivot::_enter_tree() { RETURN_IF_EDITOR }

void CameraPivot::_process(float delta) {}

void CameraPivot::process(StateContext* context, float delta) {
	RETURN_IF_EDITOR

	context->input->camera2ddir = Vector2::from_angle(-get_rotation().y - (PI_HALF));
	context->input->input_relative = context->input->input_raw.rotated(-get_rotation().y);

	switch (context->input->mode) {
		case EInputMode::JOYPAD: {
			Vector3 current_rot = get_rotation_degrees();
			Vector2 motion = context->input->input_raw;
			current_rot.y += motion.x;
			current_rot.x += motion.y;
			current_rot.x = Math::clamp(current_rot.x, X_MIN_ROTATION, X_MAX_ROTATION);
			set_rotation_degrees(current_rot);
			break;
		}
		default: break;
	}
}

void CameraPivot::_physics_process(float delta) {}

// void CameraPivot::set_length_impl(float p_length) { // TRIAL WITH EDITOR FUNCTIONALITY
// 	set_length(p_length);
// if (!mesh) {
// 	// Log(ELog::WARN, "Mesh child not present");
// 	// camera = cast_to<Camera3D>(get_child(0));
// 	// mesh = cast_to<MeshInstance3D>(get_child(1));
// 	// mesh = get_node<MeshInstance3D>("MeshInstance3D");
// 	return;
// }
// // ASSERT(camera != nullptr, "");
// const float angle = 2 * acosf(q.w);
// Vector3 dir(2 * asinf(q.x), 2 * asinf(q.y), 2 * asinf(q.z));
// Log(ELog::DEBUG, "Camera Springarm dir: ", dir.normalized());
// printf("length: %f   ", get_length());
// Log(ELog::DEBUG, ("Camera Springarm quat: %s", String(q).utf8().get_data()));
// Log(ELog::DEBUG, "Camera Springarm rot: ", rot);
// // camera->set_global_position(get_global_position() + (dir * get_length()));
// mesh->set_global_position(get_global_position() + (dir * get_length()));
// }
// float CameraPivot::get_length_impl() const { return get_length(); }

void CameraPivot::process_input(StateContext* context, float delta) {
	switch (context->input->mode) {
		case EInputMode::MOUSE_N_KEYBOARD: {
			Vector2 motion = context->input->motion;
			Vector3 current_rot = get_rotation_degrees();
			current_rot.y += motion.x * MNKMOTION_X_MULTIPLIER * delta * (MNK_X_INVERTED ? 1.f : -1.f);
			current_rot.x += motion.y * MNKMOTION_Y_MULTIPLIER * delta * (MNK_Y_INVERTED ? 1.f : -1.f);
			current_rot.x = Math::clamp(current_rot.x, X_MIN_ROTATION, X_MAX_ROTATION);
			set_rotation_degrees(current_rot);
			break;
		}
		case EInputMode::JOYPAD: {
			context->input->motion.x *= JOYMOTION_X_MULTIPLIER * delta * (JOYMOTION_X_INVERTED ? 1.f : -1.f);
			context->input->motion.y *= JOYMOTION_Y_MULTIPLIER * delta * (JOYMOTION_Y_INVERTED ? 1.f : -1.f);
			break;
		}
		default: break;
	}
}
