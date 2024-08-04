#ifndef GDEXAMPLEPLUGIN_NODE3D_EXTENSION_H
#define GDEXAMPLEPLUGIN_NODE3D_EXTENSION_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot
{

    class Node3DExtension : public Node3D
    {
    private:
        GDCLASS(Node3DExtension, Node3D)

    private:
        double time_passed;

    public:
        static void _bind_methods();
        void _init();
        void _process(double delta);

        Node3DExtension();
        ~Node3DExtension();
    };
}

#endif // GDEXAMPLEPLUGIN_NODE3D_EXTENSION_H
