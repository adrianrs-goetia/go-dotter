#ifndef GD_CHARACTER_CAMERAPIVOT_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_CHARACTER_CAMERAPIVOT_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <core/core.h>
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class PlayerNode;

class CameraPivot : public Node3D {
	GDCLASS(CameraPivot, Node3D)

public: // data
	PlayerNode* m_playernode = nullptr;

public: // functions
	GETNAME(CameraPivot)
	static void _bind_methods();

	void _enter_tree() override;
	void _ready() override;
	void _physics_process(float delta) override;
};

#endif // GD_CHARACTER_CAMERAPIVOT_PLUGIN_STEIKEMANNGAMEPLAY_H