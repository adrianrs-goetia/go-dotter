#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/curve2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>

#include <configHandler.h>

class ComponentAnimation : public godot::AnimationTree {
	GDCLASS(ComponentAnimation, godot::AnimationTree)

private:
	enum MovementAnimationType {
		Airborne,
		Walking,
	};

	enum ParryAnimationType {
		ParryHigh,
		ParryLow,
	};
	
		void setParryType(ParryAnimationType type) {
		switch (type) {
			case ParryAnimationType::ParryHigh:
				set("parameters/parry_type/blend_amount", 0);
				break;
			case ParryAnimationType::ParryLow:
				set("parameters/parry_type/blend_amount", 1);
				break; 
		}
	}

	void setMovement(MovementAnimationType type) {
		set("parameters/action_or_locomotion_upper/blend_amount", 1);
		set("parameters/action_or_locomotion_lower/blend_amount", 1);
		switch (type) {
			case MovementAnimationType::Walking:
				set("parameters/locomotion_lower/blend_amount", 0);
				set("parameters/locomotion_upper/blend_amount", 0);
				break;
			case MovementAnimationType::Airborne:
				set("parameters/locomotion_lower/blend_amount", 1);
				set("parameters/locomotion_upper/blend_amount", 1);
				break; 
		}
	}

public:
	enum EAnim : int {
		NONE = -1,
		ATTACK = 5,
	};

	GS_PATH_IMPL(m_pathToRootAnimationNode, RootAnimationNode)
	GS_PATH_IMPL(m_pathToAnimatedCharacterScene, AnimatedCharacterScene)

public:
	godot::Node3D* m_animRoot = nullptr;

	// godot::Ref<godot::PackedScene> m_attackAnimCurve;

	EAnim m_currentOneshotAnim = EAnim::NONE;
	godot::Skeleton3D* m_skeleton = nullptr;

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(ComponentAnimation, RootAnimationNode, NODE_PATH);
		METHOD_PROPERTY_IMPL(ComponentAnimation, AnimatedCharacterScene, NODE_PATH);
		// METHOD_PROPERTY_PACKEDSCENE_IMPL(ComponentAnimation, AttackAnimCurve);
	}

	void _enter_tree() override {
		if (!m_pathToAnimatedCharacterScene.is_empty()) {
			auto* node = get_node<godot::Node>(m_pathToAnimatedCharacterScene);
			set_animation_player(node->get_node<godot::AnimationPlayer>("AnimationPlayer")->get_path());

			m_skeleton = node->get_node<godot::Skeleton3D>("Armature/Skeleton3D");
			ASSERTNN(m_skeleton)
		}

		RETURN_IF_EDITOR(void())

		m_animRoot = get_node<Node3D>(m_pathToRootAnimationNode);
		ASSERTNN(m_animRoot);
	}

	void _physics_process(double delta) override {
		RETURN_IF_EDITOR(void())

		// @todo: remove this tmp get variable. Should be feed as some parameter
		// Used to spin in TPose
		if (m_currentOneshotAnim == EAnim::ATTACK) {
			m_animRoot->rotate_y(GETPARAMGLOBAL_D("player", "attack", "tmpRotation") * delta);
		}
	}

	void rotateRootTowardsVector(godot::Vector3 vector, float delta, float slerpWeight) {
		if (vector.length_squared() <= 0) {
			return;
		}
		vector.normalize();

		float angle = godot::Math::acos(g_forward.dot(vector));
		const int angle_dir = (g_right.dot(vector) > 0.f) ? 1 : -1;
		angle *= angle_dir;
		const godot::Quaternion curquat = m_animRoot->get_transform().get_basis().get_quaternion();
		const godot::Quaternion targetquat(g_up, angle);
		godot::Quaternion newquat = curquat.slerp(targetquat, delta * slerpWeight);
		m_animRoot->set_basis(godot::Basis(newquat));
	}
	
	void setRootTowardsVector(godot::Vector3 vector) {
		if (vector.length_squared() <= 0) {
			return;
		}
		vector.normalize();

		m_animRoot->set_basis(createBasisFromDirection(vector));
	}

	void doParry() {
		set("parameters/action_or_locomotion_upper/blend_amount", 0);
		// if stand still, should set 
		// set("parameters/action_or_locomotion_lower/blend_amount", 0);
		set("parameters/action_lower/blend_amount", 0);
		set("parameters/action_upper/blend_amount", 0);
	}

	void dontParry() {
		set("parameters/action_lower/blend_amount", 0);
		set("parameters/action_upper/blend_amount", 0);
	}

	void doAttack() {
		set("parameters/action_or_locomotion_upper/blend_amount", 0);
		// if stand still, should set 
		// set("parameters/action_or_locomotion_lower/blend_amount", 0);
		set("parameters/action_lower/blend_amount", 1);
		set("parameters/action_upper/blend_amount", 1);
	}

	void inAir() {
		setMovement(MovementAnimationType::Airborne);
	}

	void onGround() {
		setMovement(MovementAnimationType::Walking);
	}

	void idleRunValue(float value) {
		set("parameters/run_upper/blend_position", value);
		set("parameters/run_lower/blend_position", value);
	}

	// Tmp method for forcing TPose
	void setActive(bool active) {
		set_active(active);
		if (!active) {
			m_skeleton->reset_bone_poses();
		}
	}

	void playAnimation(EAnim anim) {
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

	// GS_PACKEDSCENE_IMPL(m_attackAnimCurve, AttackAnimCurve)
};
