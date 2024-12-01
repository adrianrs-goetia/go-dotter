#pragma once

#include "baseClasses/grapple.hpp"

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

class GrappleTargetComponent : public GrappleBaseComponent {
	GDCLASS(GrappleTargetComponent, GrappleBaseComponent)

private:
	godot::NodePath m_pathToArea3D;
	godot::Area3D* m_area = nullptr;

public:
	static void _bind_methods();

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() override;
	void _exit_tree() override;

	godot::RID getRid() const;
	godot::Vector3 getGlobalPosition() const;

public: // get/set editor-properties
	void setAreaPath(godot::NodePath path);
	godot::NodePath getAreaPath();
};
