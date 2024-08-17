#include <character/camerapivot.h>
#include <character/playernode.h>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;

constexpr float X_ACCELERATION_LERP = 3.4f;
constexpr float X_LERP_MAX_DIST = 8.f;
constexpr float MIN_Y_HEIGHT = -3.f;

void CameraPivot::_bind_methods() { DEFAULT_PROPERTY(CameraPivot) }

void CameraPivot::_enter_tree() {}

void CameraPivot::_ready() {
	RETURN_IF_EDITOR
	m_playernode = get_node<PlayerNode>(NodePath("/root/Main/Pottit_Player"));
	if (!m_playernode) {
		printf("%s camera pivot failed to fetch playernode \n", __FUNCTION__);
	}
}

void CameraPivot::_physics_process(float delta) {
	RETURN_IF_EDITOR
	Vector3 position = get_position();
	position.z = 0;

	if (m_playernode) {
		const Vector3 player_position = m_playernode->get_position();
		float x_player = player_position.x;
		float x_lerp = x_player;
		const Vector3 player_velocity = m_playernode->get_velocity();
		x_lerp += player_velocity.x * X_ACCELERATION_LERP;
		x_lerp = Math::clamp(x_lerp, x_player - X_LERP_MAX_DIST, x_player + X_LERP_MAX_DIST);

		Vector2 input_dir = Input::get_singleton()->get_vector("move_left", "move_right", "ui_up", "ui_down", 0.05);
		if (Math::abs(input_dir.x) || !m_playernode->is_on_floor()) {
			position.x = Math::lerp(position.x, x_lerp, delta);
		}
		position.y = Math::max(Math::lerp(position.y, player_position.y, delta), MIN_Y_HEIGHT);
		set_position(position);
	}
}
