#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

class AnimationComponent : public NodeComponent {
	GDCLASS(AnimationComponent, NodeComponent)

public:
	godot::NodePath m_pathToRootAnimationNode;
	Node3D* m_animRoot = nullptr;

public:
	static void _bind_methods();

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() override;
	void _exit_tree() override;

	void rotateRootTowardsVector(godot::Vector3 vector, float delta, float slerpWeight);

	GS_PATH_IMPL(m_pathToRootAnimationNode, RootAnimationNode)
};
