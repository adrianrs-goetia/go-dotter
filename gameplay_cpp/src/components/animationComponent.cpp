#include "animationComponent.h"

void AnimationComponent::_bind_methods() {
	METHOD_PATH_IMPL(AnimationComponent, m_pathToRootAnimationNode, "Root node path");
}

void AnimationComponent::_enter_tree() {
}

void AnimationComponent::_exit_tree() {
}
