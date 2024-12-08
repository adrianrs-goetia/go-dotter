#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

class AnimationComponent : public godot::AnimationTree {
	GDCLASS(AnimationComponent, godot::AnimationTree)

public:
	enum EPlayingAnim : int {
		NONE = -1,
		ATTACK = 5,
	};

	godot::NodePath m_pathToRootAnimationNode;
	godot::NodePath m_pathToAnimatedCharacterScene;
	godot::Node3D* m_animRoot = nullptr;

	godot::Ref<godot::PackedScene> m_attackAnimCurve;

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;

	void rotateRootTowardsVector(godot::Vector3 vector, float delta, float slerpWeight);

	void playAnimation(EPlayingAnim anim);

	GS_PATH_IMPL(m_pathToRootAnimationNode, RootAnimationNode)
	GS_PATH_IMPL(m_pathToAnimatedCharacterScene, AnimatedCharacterScene)
	// @todo Animations can be a dictionary
	// Assumes specialized animation components are inherited from this
	GS_PACKEDSCENE_IMPL(m_attackAnimCurve, AttackAnimCurve)
};
