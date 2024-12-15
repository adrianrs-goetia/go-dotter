#include "animationComponent.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/curve2d.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>

#include <configHandler.h>

using namespace godot;

void AnimationComponent::_bind_methods() {
	METHOD_PROPERTY_IMPL(AnimationComponent, RootAnimationNode, NODE_PATH);
	METHOD_PROPERTY_IMPL(AnimationComponent, AnimatedCharacterScene, NODE_PATH);
	// METHOD_PROPERTY_PACKEDSCENE_IMPL(AnimationComponent, AttackAnimCurve);
}

void AnimationComponent::_enter_tree() {
	if (!m_pathToAnimatedCharacterScene.is_empty()) {
		auto* node = get_node<Node>(m_pathToAnimatedCharacterScene);
		set_animation_player(node->get_node<AnimationPlayer>("AnimationPlayer")->get_path());

		m_skeleton = node->get_node<Skeleton3D>("Armature/Skeleton3D");
		ASSERT_NOTNULL(m_skeleton)
	}

	RETURN_IF_EDITOR(void())

	m_animRoot = get_node<Node3D>(m_pathToRootAnimationNode);
	ASSERT_NOTNULL(m_animRoot);
}

void AnimationComponent::_exit_tree() {}

void AnimationComponent::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	if (m_currentOneshotAnim == EAnim::ATTACK) {
		m_animRoot->rotate_y(GETPARAMGLOBAL_D("player", "attack", "tmpRotation") * delta);
	}
}

void AnimationComponent::rotateRootTowardsVector(godot::Vector3 vector, float delta, float slerpWeight) {
	if (vector.length_squared() <= 0) {
		return;
	}
	vector.normalize();

	float angle = Math::acos(g_forward.dot(vector));
	const int angle_dir = (g_right.dot(vector) > 0.f) ? 1 : -1;
	angle *= angle_dir;
	const Quaternion curquat = m_animRoot->get_transform().get_basis().get_quaternion();
	const Quaternion targetquat(g_up, angle);
	Quaternion newquat = curquat.slerp(targetquat, delta * slerpWeight);
	m_animRoot->set_basis(Basis(newquat));
}

void AnimationComponent::setRootTowardsVector(godot::Vector3 vector) {
	if (vector.length_squared() <= 0) {
		return;
	}
	vector.normalize();

	m_animRoot->set_basis(createBasisFromDirection(vector));
}

void AnimationComponent::idleRunValue(float value) {
	set("parameters/blend_position", value);
}

void AnimationComponent::setActive(bool active) {
	set_active(active);
	if (!active) {
		m_skeleton->reset_bone_poses();
	}
}

void AnimationComponent::playAnimation(EAnim anim) {
	m_currentOneshotAnim = anim;
	switch (anim) {
		case EAnim::NONE: {
			setActive(true);
			break;
		}
		case EAnim::ATTACK: {
			setActive(false); // tmp method, force TPose during attack
			break;
		}
		default:
			break;
	}
}
