#include <character/camerapivot.h>
#include <character/playernode.h>
#include <environment/grapplenode.h>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_action.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/viewport.hpp>

extern "C" {
#include <stdlib.h>
}

constexpr float MESHDUMMY_ROTATIONSPEED = 18.f;

using namespace godot;

void PlayerNode::_bind_methods() {
	DEFAULT_PROPERTY(PlayerNode)
	ClassDB::bind_method(D_METHOD("area_entered_grappledetection", "area"), &PlayerNode::area_entered_grappledetection);
	ClassDB::bind_method(D_METHOD("area_exited_grappledetection", "area"), &PlayerNode::area_exited_grappledetection);
}

void PlayerNode::_enter_tree() {
	Log(ELog::DEBUG, "PlayerNode entering tree -- editor");

	RETURN_IF_EDITOR
	Log(ELog::DEBUG, "PlayerNode entering tree");

	meshdummy = get_node<Node3D>("dummy");
	ASSERT(meshdummy != nullptr, "");

	m_grappledetectionarea = get_node<Area3D>("GrappleDetection");
	ASSERT(m_grappledetectionarea != nullptr, "");
	m_grappledetectionarea->connect("area_entered", callable_mp(this, &PlayerNode::area_entered_grappledetection));
	m_grappledetectionarea->connect("area_exited", callable_mp(this, &PlayerNode::area_exited_grappledetection));

	m_state_context = (StateContext*)calloc(1, sizeof(StateContext));
	ASSERT(m_state_context != nullptr, "");

	m_camerapivot = get_node<CameraPivot>(NodePaths::camera_pivot);
	ASSERT(m_camerapivot != nullptr, "");

	m_state_context->physics.is_on_ground = is_on_floor();
	m_state_context->physics.position = get_position();
	m_state_context->physics.velocity = get_velocity();
	m_fsm.force_set_state<PlayerInAirState>(m_state_context);
}

void PlayerNode::_exit_tree() {
	RETURN_IF_EDITOR
	Log(ELog::DEBUG, "PlayerNode exiting tree");
	::free(m_state_context);

	ASSERT(m_grappledetectionarea != nullptr, "");
	m_grappledetectionarea->disconnect("area_entered", callable_mp(this, &PlayerNode::area_entered_grappledetection));
	m_grappledetectionarea->disconnect("area_exited", callable_mp(this, &PlayerNode::area_exited_grappledetection));
	m_grappledetectionarea = nullptr;

	m_state_context = nullptr;
}

void PlayerNode::_process(float delta) {
	RETURN_IF_EDITOR
	ASSERT(m_camerapivot != nullptr, "");
	m_camerapivot->process(m_state_context, delta);
	determine_grapple_target();

	const Vector2 c = m_state_context->input.camera2ddir.normalized().rotated(-Math::deg_to_rad(90.f));
	const Vector3 cam3d = Vector3(c.x, 0, c.y).normalized();
	DEBUG_DRAW_LINE(get_position(), get_position() + (cam3d * 10.f), Color(1, 1, 1));
	if (m_state_context->grapple.target) {
		DEBUG_DRAW_POSITION(
				Transform3D(Basis(Vector3(0, 1, 0), 0, Vector3(3, 3, 3)), m_state_context->grapple.target_position),
				Color(0, 0, 1), delta);
		DEBUG_DRAW_LINE(get_position(), m_state_context->grapple.target_position, Color(0, 1, 0));
	}
}

void PlayerNode::_physics_process(float delta) {
	RETURN_IF_EDITOR
	ASSERT(m_state_context != nullptr, "");
	// capture current physics context
	m_state_context->physics.is_on_ground = is_on_floor();
	m_state_context->physics.position = get_position();
	m_state_context->physics.velocity = get_velocity();

	// Let FSM deal with physics and input context
	m_fsm.physics_process(m_state_context, delta);
	m_fsm.handle_input(m_state_context, delta);

	// set data from context
	set_velocity(m_state_context->physics.velocity);
	move_and_slide();
	rotate_towards_velocity(delta);

	// deferred actions
	m_fsm.deferred_actions(m_state_context);
}

void PlayerNode::_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR
	if (!m_state_context) { return; }

	const float delta = get_process_delta_time();
	Input* input = Input::get_singleton();

	m_state_context->input.movedir = input->get_vector(
			InputMap::move_left, InputMap::move_right, InputMap::move_forward, InputMap::move_backward, 0.05);
	if (p_event->is_action_pressed(InputMap::jump)) {
		m_state_context->input.input_action = InputAction{ EInputAction::JUMP, EInputActionType::PRESSED };
	}
	else {
		m_state_context->input.last_valid_input_action = m_state_context->input.input_action;
		m_state_context->input.input_action = InputAction{ EInputAction::NONE, EInputActionType::NONE };
	}

	ASSERT(m_camerapivot != nullptr, "");
	m_camerapivot->process_input(m_state_context, input, p_event, delta);
}

void PlayerNode::rotate_towards_velocity(float delta) {
	const Vector3 g_up(0, 1, 0);
	const Vector3 g_forward(0, 0, 1);
	const Vector3 g_right(1, 0, 0);
	Vector3 input(m_state_context->input.movedir_rotated.x, 0, m_state_context->input.movedir_rotated.y);
	if (input.length_squared() <= 0) { return; }
	input.normalize();

	const int angle_dir = (g_right.dot(input) > 0.f) ? 1 : -1;
	float angle = Math::acos(g_forward.dot(input));
	angle *= angle_dir;
	const Quaternion curquat = meshdummy->get_transform().get_basis().get_quaternion();
	const Quaternion targetquat(g_up, angle);
	Quaternion newquat = curquat.slerp(targetquat, delta * MESHDUMMY_ROTATIONSPEED);
	meshdummy->set_basis(Basis(newquat));
}


void PlayerNode::area_entered_grappledetection(Area3D* area) {
	LOG(DEBUG, "Area entered grapple area: ", area->get_name())
	if (GrappleNode* gn = cast_to<GrappleNode>(area)) { m_in_range_grapplenodes.push_back(gn); }
}

void PlayerNode::area_exited_grappledetection(Area3D* area) {
	LOG(DEBUG, "Area left grapple area: ", area->get_name())
	if (GrappleNode* gn = cast_to<GrappleNode>(area)) {
		auto it = std::find_if(m_in_range_grapplenodes.begin(), m_in_range_grapplenodes.end(),
				[gn](GrappleNode* a) -> bool { return a->get_rid() == gn->get_rid(); });
		m_in_range_grapplenodes.erase(it);
		if (gn == m_state_context->grapple.target) { m_state_context->grapple = { nullptr, Vector3() }; }
	}
}

void PlayerNode::determine_grapple_target() {
	const Vector2 cam_dir = m_state_context->input.camera2ddir.normalized().rotated(-Math::deg_to_rad(90.f));
	float lowest_dot = -1.0f;
	GrappleNode* target = nullptr;
	for (GrappleNode* gn : m_in_range_grapplenodes) {
		const Vector3 dir3d = gn->get_position() - get_position();
		const Vector2 gn_unit_dir = Vector2(dir3d.x, dir3d.z).normalized();
		const float dot = cam_dir.dot(gn_unit_dir);
		if (dot > lowest_dot && dot > 0.f) {
			lowest_dot = dot;
			target = gn;
		}
	}
	if (target) m_state_context->grapple = { target, target->get_position() };
	else m_state_context->grapple = { nullptr, Vector3() };
}
