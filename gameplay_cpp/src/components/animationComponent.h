#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot {
class Skeleton3D;
}

class AnimationComponent : public godot::AnimationTree {
	GDCLASS(AnimationComponent, godot::AnimationTree)

public:
	enum EAnim : int {
		NONE = -1,
		ATTACK = 5,
	};

	godot::NodePath m_pathToRootAnimationNode;
	godot::NodePath m_pathToAnimatedCharacterScene;
	godot::Node3D* m_animRoot = nullptr;

	// godot::Ref<godot::PackedScene> m_attackAnimCurve;

	EAnim m_currentOneshotAnim = EAnim::NONE;
	godot::Skeleton3D* m_skeleton = nullptr;

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;
	void _physics_process(double delta) override;

	void rotateRootTowardsVector(godot::Vector3 vector, float delta, float slerpWeight);

	void idleRunValue(float value);
	void setActive(bool active);
	void playAnimation(EAnim anim);

	GS_PATH_IMPL(m_pathToRootAnimationNode, RootAnimationNode)
	GS_PATH_IMPL(m_pathToAnimatedCharacterScene, AnimatedCharacterScene)
	// GS_PACKEDSCENE_IMPL(m_attackAnimCurve, AttackAnimCurve)
};
