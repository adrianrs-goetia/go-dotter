#pragma once

#include "baseClasses/grappleBase.hpp"

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

class GrappleTargetComponent : public ComponentGrappleBase {
	GDCLASS(GrappleTargetComponent, ComponentGrappleBase)

private:
	GS_PATH_IMPL(m_colliderPath, ColliderPath)

	godot::Area3D* m_area = nullptr;

public:
	static void _bind_methods();

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() override;
	void _exit_tree() override;

	godot::RID getRid() const;
};
