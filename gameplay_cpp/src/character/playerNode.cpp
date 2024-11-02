#include <character/cameraPivot.h>
#include <character/playerNode.h>
#include <components/grappleComponent.h>
#include <components/inputComponent.h>
#include <components/parryInstigatorComponent.h>

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
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
	ClassDB::bind_method(D_METHOD("areaEnteredGrappledetection", "area"), &PlayerNode::areaEnteredGrappledetection);
	ClassDB::bind_method(D_METHOD("areaExitedGrappledetection", "area"), &PlayerNode::areaExitedGrappledetection);
}

void PlayerNode::_notification(int what) {
	// LOG(INFO, "Notification", (int64_t)what)
	switch (what) {
		case NOTIFICATION_WM_CLOSE_REQUEST: {
			LOG(INFO, "PlayerNode WM_CLOSE_REQUEST")
			// exit_game();
			break;
		}

		default: break;
	}
}

void PlayerNode::_enter_tree() {
	Log(ELog::DEBUG, "PlayerNode entering tree -- editor");

	m_grappleComponent = getChildOfNode<GrappleComponent>(this);
	m_parryComponent = getChildOfNode<ParryInstigatorComponent>(this);
	auto* audio = getChildOfNode<AudioStreamPlayer3D>(this);
	auto* particles = getChildOfNode<GPUParticles3D>(this);

	RETURN_IF_EDITOR(void())
	Log(ELog::DEBUG, "PlayerNode entering tree");

	m_stateContext = (StateContext*)calloc(1, sizeof(StateContext));
	m_meshAnchor = get_node<Node3D>("meshAnchor");
	m_grappleDetectionArea = get_node<Area3D>("GrappleDetection");
	m_camerapivot = get_node<CameraPivot>(nodePaths::cameraPivot);

	ASSERT_NOTNULL(m_grappleComponent)
	ASSERT_NOTNULL(m_parryComponent)
	ASSERT_NOTNULL(m_stateContext)
	ASSERT_NOTNULL(m_grappleDetectionArea)
	ASSERT_NOTNULL(m_camerapivot)
	ASSERT_NOTNULL(m_meshAnchor)
	ASSERT_NOTNULL(audio)
	ASSERT_NOTNULL(particles)

	m_grappleDetectionArea->connect("area_entered", callable_mp(this, &PlayerNode::areaEnteredGrappledetection));
	m_grappleDetectionArea->connect("area_exited", callable_mp(this, &PlayerNode::areaExitedGrappledetection));

	m_stateContext->input = InputComponent::getinput(this);
	m_stateContext->parry = m_parryComponent;
	m_stateContext->physics.is_on_ground = is_on_floor();
	m_stateContext->physics.position = get_position();
	m_stateContext->physics.velocity = get_velocity();
	m_stateContext->audioVisual.audio = audio;
	m_stateContext->audioVisual.particles = particles;
	m_fsm.init(*m_stateContext, PlayerStateBank::get().state<PlayerInAirState>());

	m_stateContext->grapple.instigator = m_grappleComponent;

	m_parryComponent->m_getDesiredDirectionCb = [this]() -> Vector3 {
		Vector3 desiredDir = m_stateContext->input->getInputRelative3d();
		if (desiredDir.length_squared() < 0.2f) { desiredDir = m_stateContext->input->getCamera3dDir(); }
		// desiredDir is flipped to launch player and object in the opposite directions
		return desiredDir * -1.f;
	};
}

void PlayerNode::_exit_tree() {
	RETURN_IF_EDITOR(void())
	Log(ELog::DEBUG, "PlayerNode exiting tree");

	ASSERT_NOTNULL(m_grappleDetectionArea);
	ASSERT_NOTNULL(m_stateContext);

	::free(m_stateContext);

	m_grappleDetectionArea->disconnect("area_entered", callable_mp(this, &PlayerNode::areaEnteredGrappledetection));
	m_grappleDetectionArea->disconnect("area_exited", callable_mp(this, &PlayerNode::areaExitedGrappledetection));

	m_grappleDetectionArea = nullptr;
	m_stateContext = nullptr;
}

void PlayerNode::_process(double delta) {
	RETURN_IF_EDITOR(void())
	ASSERT_NOTNULL(m_camerapivot);
	m_camerapivot->process(*m_stateContext, delta);
	determineGrappleTarget();

	if (m_stateContext->grapple.target) {
		DebugDraw::Position(Transform3D(Basis(Vector3(0, 1, 0), 0, Vector3(3, 3, 3)),
									m_stateContext->grapple.target->get_global_position()),
				Color(0, 0, 1), delta);
		DebugDraw::Line(get_position(), m_stateContext->grapple.target->get_global_position(), Color(0, 1, 0));
	}
}

void PlayerNode::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())
	ASSERT_NOTNULL(m_stateContext);
	// capture current physics context
	m_stateContext->physics.is_on_ground = is_on_floor();
	m_stateContext->physics.position = get_position();
	m_stateContext->physics.velocity = get_velocity();

	// Let FSM deal with physics and input context
	m_fsm.physicsProcess(*m_stateContext, delta);
	m_fsm.handleInput(*m_stateContext, delta);

	// set data from context
	set_velocity(m_stateContext->physics.velocity);
	move_and_slide();
	rotateTowardsVelocity(delta);

	// deferred actions
	m_fsm.deferredActions(*m_stateContext);
}

void PlayerNode::_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR(void())
	if (!m_stateContext) { return; }
	ASSERT_NOTNULL(m_camerapivot);
	m_camerapivot->processInput(*m_stateContext, get_process_delta_time());
}

void PlayerNode::rotateTowardsVelocity(float delta) {
	const Vector2 input_relative = m_stateContext->input->m_inputCameraRelative;
	Vector3 inputvec(input_relative.x, 0, input_relative.y);
	if (inputvec.length_squared() <= 0) { return; }
	inputvec.normalize();

	float angle = Math::acos(g_forward.dot(inputvec));
	const int angle_dir = (g_right.dot(inputvec) > 0.f) ? 1 : -1;
	angle *= angle_dir;
	const Quaternion curquat = m_meshAnchor->get_transform().get_basis().get_quaternion();
	const Quaternion targetquat(g_up, angle);
	Quaternion newquat = curquat.slerp(targetquat, delta * MESHDUMMY_ROTATIONSPEED);
	m_meshAnchor->set_basis(Basis(newquat));
}

void PlayerNode::areaEnteredGrappledetection(Area3D* area) {
	RETURN_IF_EDITOR(void())
	if (area->get_rid() == m_grappleComponent->getRid()) { return; }
	if (auto* gn = getAdjacentNode<GrappleComponent>(area)) {
		LOG(DEBUG, "Component entered grapple area: ", gn->get_name())
		m_inRangeGrapplenodes.push_back(gn);
	}
}

void PlayerNode::areaExitedGrappledetection(Area3D* area) {
	RETURN_IF_EDITOR(void())
	if (auto* gn = getAdjacentNode<GrappleComponent>(area)) {
		LOG(DEBUG, "Node left grapple area: ", area->get_parent()->get_name())
		auto it = std::find_if(m_inRangeGrapplenodes.begin(), m_inRangeGrapplenodes.end(),
				[gn](GrappleComponent* a) -> bool { return a->getRid() == gn->getRid(); });
		m_inRangeGrapplenodes.erase(it);
		if (gn == m_stateContext->grapple.target) { m_stateContext->grapple = { m_grappleComponent, nullptr }; }
	}
}

void PlayerNode::determineGrappleTarget() {
	RETURN_IF_EDITOR(void())
	ASSERT_NOTNULL(m_stateContext)
	const Vector3 cam3d = m_stateContext->input->getCamera3dDir();
	float lowest_dot = -1.0f;
	GrappleComponent* target = nullptr;
	for (GrappleComponent* gn : m_inRangeGrapplenodes) {
		Vector3 dir_2d = gn->get_global_position() - get_global_position();
		dir_2d.y = 0;
		dir_2d.normalize();
		const float dot = cam3d.dot(dir_2d);
		if (dot > lowest_dot && dot > 0.f) {
			lowest_dot = dot;
			target = gn;
		}
	}
	if (target) m_stateContext->grapple = { m_grappleComponent, target };
	else m_stateContext->grapple = { m_grappleComponent, nullptr };
}
