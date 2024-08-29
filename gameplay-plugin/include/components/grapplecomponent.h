#pragma once

#include <nodecomponent.hpp>

using namespace godot;

struct GrappleEvent;

/**
 * intermediary class for typechecking all grappling components
 */
class GrappleComponent : public NodeComponent {
	GDCLASS(GrappleComponent, NodeComponent)
private:
	bool is_anchor = true;

public:
	static void _bind_methods();

	void set_is_anchor(bool val);
	bool get_is_anchor() const;
};

class GrappleEvent {
public:
	GrappleComponent* _initiator = nullptr;
	GrappleComponent* _subject = nullptr;
	float _launch_strength = 0.f;
	float _launch_weight = 0.5f; // 0-1 of launch distribution between initiator and subject

public:
	/**
	 * Launches non-anchor Node towards anchor
	 * Weight is ignored when one of the nodes is an anchor
	 *
	 * Logs warning of both are anchor and does nothing
	 *
	 * Two non-anchored nodes are launched towards each other with the weight as the linear
	 * interpolation variable:
	 * (1-weight) * initiator_launch_strength
	 * (weight) * subject_launch_strength
	 */
	void launch();
};
