#include "node3d_extension.h"

void godot::Node3DExtension::_bind_methods()
{
}

void godot::Node3DExtension::_init()
{
}

void godot::Node3DExtension::_process(double delta)
{
    time_passed += delta;
    Vector3 new_position = Vector3(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)), 0.0);
    // Vector3 new_position = Vector3(10.0 + (10.0 * sin(time_passed * 2.0)), 1.0 + (1.0 * cos(time_passed * 1.5)), 0.0);
    set_position(new_position);
}

godot::Node3DExtension::Node3DExtension()
{
    time_passed = 0.0;
}

godot::Node3DExtension::~Node3DExtension()
{
}
