#pragma once

#include <core/core.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include <sstream>

class PlayerNode;

class GamesceneBillboard : public godot::Node3D {
	GDTYPE(GamesceneBillboard, godot::Node3D)

	struct Path {
		static godot::String area3d() {
			return "Area3D";
		}
		static godot::String collisionshape() {
			return "CollisionShape3D";
		}
		static godot::String collisionshapepath() {
			return area3d() + godot::String("/") + collisionshape();
		}
	};

	GS_PATH_IMPL(m_textPath, TextPath)
	GS_PATH_IMPL(m_subviewportPath, SubviewportPath)
	GS_PATH_IMPL(m_canvasPath, CanvasPath)
	G_STRING_IMPL(m_billboardText, BillboardText, = "THIS IS TEXT")
	G_VECTOR2I_IMPL(m_billboardSize, BillboardSize, = godot::Vector2(150, 100))

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(GamesceneBillboard, TextPath, NODE_PATH)
		METHOD_PROPERTY_IMPL(GamesceneBillboard, CanvasPath, NODE_PATH)
		METHOD_PROPERTY_IMPL(GamesceneBillboard, SubviewportPath, NODE_PATH)
		METHOD_PROPERTY_IMPL(GamesceneBillboard, BillboardText, STRING)
		METHOD_PROPERTY_IMPL(GamesceneBillboard, BillboardSize, VECTOR2)
	}

	void _enter_tree() {
		RETURN_IF_EDITOR(void())
		auto* area = get_node<godot::Area3D>(Path::area3d());
		ASSERTNN(area)
		AREACONNECT_BODY(GamesceneBillboard, area, PlayerDetection)

		auto* shape = get_node<godot::CollisionShape3D>(Path::collisionshapepath());
		ASSERTNN(shape)

		area->set_name(Path::area3d());
		shape->set_name(Path::collisionshape());

		setCanvasVisibility(false);
	}

	void enteredPlayerDetection(godot::PhysicsBody3D* body) {
		if (cast_to<PlayerNode>(body)) {
			setCanvasVisibility(true);
		}
	}

	void exitedPlayerDetection(godot::PhysicsBody3D* body) {
		if (cast_to<PlayerNode>(body)) {
			setCanvasVisibility(false);
		}
	}

	void setBillboardText(godot::String string) {
		m_billboardText = string;
		RETURN_IF_NODE_NOT_READY(void())
		if (auto* text = get_node<godot::RichTextLabel>(m_textPath)) {
			text->set_text("[center]" + string + "[/center]");
		}
	}

	void setBillboardSize(godot::Vector2i size) {
		m_billboardSize = size;
		RETURN_IF_NODE_NOT_READY(void())
		if (auto* subviewport = get_node<godot::SubViewport>(m_subviewportPath)) {
			subviewport->set_size(size);
		}
	}

	void setCanvasVisibility(bool visibility) {
		auto* canvas = get_node<Node3D>(m_canvasPath);
		ASSERTNN(canvas)
		canvas->set_visible(visibility);
	}
};
