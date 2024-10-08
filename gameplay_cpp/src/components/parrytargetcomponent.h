#pragma once

#include <core/core.h>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/area3d.hpp>

using namespace godot;

class ParryTargetComponent : public Node3D {
    GDCLASS(ParryTargetComponent, Node3D)

public:
    NodePath m_pathToArea3D;
    Area3D* m_areaPtr = nullptr;

public:
    GETNAME(ParryTargetComponent)
    static void _bind_methods();

    void _enter_tree() override;

    void setAreaPath(NodePath path);
    NodePath getAreaPath();
};