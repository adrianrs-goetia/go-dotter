#include "animationComponent.h"

void AnimationComponent::_bind_methods() {
	METHOD_PROPERTY_IMPL(AnimationComponent, RootAnimationNode, NODE_PATH, "Root node path");
}

void AnimationComponent::_enter_tree() {}

void AnimationComponent::_exit_tree() {}
