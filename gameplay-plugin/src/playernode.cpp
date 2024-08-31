#include <character/camerapivot.h>
#include <character/playernode.h>
#include <components/grapplecomponent.h>
#include <components/inputcomponent.h>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_action.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <debugdraw3d/api.h>

extern "C" {
#include <stdlib.h>
}

constexpr float MESHDUMMY_ROTATIONSPEED = 18.f;

using namespace godot;

void PlayerNode::_bind_methods() {
	DEFAULT_PROPERTY(PlayerNode)
	ClassDB::bind_method(D_METHOD("area_entered_grappledetection", "area"), &PlayerNode::area_entered_grappledetection);
	ClassDB::bind_method(D_METHOD("area_exited_grappledetection", "area"), &PlayerNode::area_exited_grappledetection);
	ClassDB::bind_method(D_METHOD("area_entered_parrydetection", "area"), &PlayerNode::area_entered_parrydetection);
	ClassDB::bind_method(D_METHOD("area_exited_parrydetection", "area"), &PlayerNode::area_exited_parrydetection);
}

void PlayerNode::_enter_tree() {
	Log(ELog::DEBUG, "PlayerNode entering tree -- editor");

	m_grapplecomponent = get_child_node_of_type<GrappleComponent>(this);

	RETURN_IF_EDITOR
	Log(ELog::DEBUG, "PlayerNode entering tree");

	m_state_context = (StateContext*)calloc(1, sizeof(StateContext));
	meshdummy = get_node<Node3D>("dummy");
	m_capsule = get_node<CollisionShape3D>("Capsule");
	m_grappledetectionarea = get_node<Area3D>("GrappleDetection");
	m_parrydetectionarea = get_node<Area3D>("ParryDetection");
	m_parrydetectionshape = get_node<CollisionShape3D>("ParryDetection/CollisionShape3D");
	m_camerapivot = get_node<CameraPivot>(NodePaths::camera_pivot);

	ASSERT(m_state_context != nullptr, "")
	ASSERT(meshdummy != nullptr, "")
	ASSERT(m_capsule != nullptr, "")
	ASSERT(m_grappledetectionarea != nullptr, "")
	ASSERT(m_parrydetectionarea != nullptr, "")
	ASSERT(m_camerapivot != nullptr, "")

	m_grappledetectionarea->connect("area_entered", callable_mp(this, &PlayerNode::area_entered_grappledetection));
	m_grappledetectionarea->connect("area_exited", callable_mp(this, &PlayerNode::area_exited_grappledetection));
	m_parrydetectionarea->connect("area_entered", callable_mp(this, &PlayerNode::area_entered_parrydetection));
	m_parrydetectionarea->connect("area_exited", callable_mp(this, &PlayerNode::area_exited_parrydetection));

	m_state_context->input = InputComponent::getinput(this);
	m_state_context->physics.is_on_ground = is_on_floor();
	m_state_context->physics.position = get_position();
	m_state_context->physics.velocity = get_velocity();
	m_fsm.force_set_state<PlayerInAirState>(m_state_context);

	ASSERT(m_grapplecomponent != nullptr, "")
	m_state_context->grapple.instigator = m_grapplecomponent;

	m_parrydetectionarea->set_position(m_state_context->physics.get_gravity_center());
	auto sphere = cast_to<SphereShape3D>(*m_parrydetectionshape->get_shape());
	m_state_context->parry.detectionradius = sphere->get_radius();
	m_state_context->parry.get_rid = [this]() {
		TypedArray<RID> ignores;
		ignores.append(this->get_rid());
		ignores.append(this->m_grappledetectionarea->get_rid());
		ignores.append(this->m_parrydetectionarea->get_rid());
		return ignores;
	};
	m_state_context->parry.get_shape = [this]() { return this->m_parrydetectionshape->get_shape(); };
	m_state_context->parry.get_world = [this]() { return this->get_viewport()->get_world_3d(); };
}

void PlayerNode::_exit_tree() {
	RETURN_IF_EDITOR
	Log(ELog::DEBUG, "PlayerNode exiting tree");

	ASSERT(m_grappledetectionarea != nullptr, "");
	ASSERT(m_parrydetectionarea != nullptr, "");
	ASSERT(m_state_context != nullptr, "");

	::free(m_state_context);

	m_grappledetectionarea->disconnect("area_entered", callable_mp(this, &PlayerNode::area_entered_grappledetection));
	m_grappledetectionarea->disconnect("area_exited", callable_mp(this, &PlayerNode::area_exited_grappledetection));
	m_parrydetectionarea->disconnect("area_entered", callable_mp(this, &PlayerNode::area_entered_parrydetection));
	m_parrydetectionarea->disconnect("area_exited", callable_mp(this, &PlayerNode::area_exited_parrydetection));

	m_grappledetectionarea = nullptr;
	m_parrydetectionarea = nullptr;
	m_state_context = nullptr;
}

void PlayerNode::_process(float delta) {
	RETURN_IF_EDITOR
	ASSERT(m_camerapivot != nullptr, "");
	m_camerapivot->process(m_state_context, delta);
	determine_grapple_target();

	if (m_state_context->grapple.target) {
		DebugDraw::Position(Transform3D(Basis(Vector3(0, 1, 0), 0, Vector3(3, 3, 3)),
									m_state_context->grapple.target->get_global_position()),
				Color(0, 0, 1), delta);
		DebugDraw::Line(get_position(), m_state_context->grapple.target->get_global_position(), Color(0, 1, 0));
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

	// m_animtree.process_animation(m_state_context, delta);

	// SHAPE COLLISION WITH PHYSICS QUERY
	ASSERT(m_parrydetectionshape != nullptr, "")
}

void PlayerNode::_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR
	if (!m_state_context) { return; }
	ASSERT(m_camerapivot != nullptr, "");
	m_camerapivot->process_input(m_state_context, get_process_delta_time());
}

void PlayerNode::rotate_towards_velocity(float delta) {
	const Vector3 g_up(0, 1, 0);
	const Vector3 g_forward(0, 0, 1);
	const Vector3 g_right(1, 0, 0);
	const Vector2 input_relative = m_state_context->input->input_relative;
	Vector3 inputvec(input_relative.x, 0, input_relative.y);
	if (inputvec.length_squared() <= 0) { return; }
	inputvec.normalize();

	const int angle_dir = (g_right.dot(inputvec) > 0.f) ? 1 : -1;
	float angle = Math::acos(g_forward.dot(inputvec));
	angle *= angle_dir;
	const Quaternion curquat = meshdummy->get_transform().get_basis().get_quaternion();
	const Quaternion targetquat(g_up, angle);
	Quaternion newquat = curquat.slerp(targetquat, delta * MESHDUMMY_ROTATIONSPEED);
	meshdummy->set_basis(Basis(newquat));
}

void PlayerNode::area_entered_grappledetection(Area3D* area) {
	RETURN_IF_EDITOR
	if (area->get_rid() == m_grapplecomponent->get_rid()) { return; }
	if (auto* gn = cast_to<GrappleComponent>(area->get_parent())) {
		LOG(DEBUG, "Component entered grapple area: ", gn->get_name())
		m_in_range_grapplenodes.push_back(gn);
	}
}

void PlayerNode::area_exited_grappledetection(Area3D* area) {
	RETURN_IF_EDITOR
	LOG(DEBUG, "Area left grapple area: ", area->get_name())
	if (auto* gn = cast_to<GrappleComponent>(area->get_parent())) {
		auto it = std::find_if(m_in_range_grapplenodes.begin(), m_in_range_grapplenodes.end(),
				[gn](GrappleComponent* a) -> bool { return a->get_rid() == gn->get_rid(); });
		m_in_range_grapplenodes.erase(it);
		if (gn == m_state_context->grapple.target) { m_state_context->grapple = { m_grapplecomponent, nullptr }; }
	}
}

void PlayerNode::determine_grapple_target() {
	RETURN_IF_EDITOR
	ASSERT(m_state_context != nullptr, "")
	const Vector3 cam3d = m_state_context->input->get_camera3ddir();
	float lowest_dot = -1.0f;
	GrappleComponent* target = nullptr;
	for (GrappleComponent* gn : m_in_range_grapplenodes) {
		Vector3 dir_2d = gn->get_global_position() - get_global_position();
		dir_2d.y = 0;
		dir_2d.normalize();
		const float dot = cam3d.dot(dir_2d);
		if (dot > lowest_dot && dot > 0.f) {
			lowest_dot = dot;
			target = gn;
		}
	}
	if (target) m_state_context->grapple = { m_grapplecomponent, target };
	else m_state_context->grapple = { m_grapplecomponent, nullptr };
}

void PlayerNode::area_entered_parrydetection(Area3D* area) { RETURN_IF_EDITOR }

void PlayerNode::area_exited_parrydetection(Area3D* area) {}
