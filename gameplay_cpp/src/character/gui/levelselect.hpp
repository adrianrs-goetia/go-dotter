#pragma once

#include "utils/button.hpp"
#include <core/core.hpp>

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

class LevelSelect : public godot::ScrollContainer {
	GDTYPE(LevelSelect, godot::ScrollContainer)

	GS_PATH_IMPL(levelspath, LevelPath, = "res://gameplayscenes/")

	godot::VBoxContainer* vbox = nullptr;

public:
	static void _bind_methods() {}

	void _enter_tree() override {
		// Create initial vertical box
		vbox = getComponentOfNode<godot::VBoxContainer>(this);
		if (!vbox) {
			vbox = memnew(godot::VBoxContainer);
			add_child(vbox);
			vbox->set_owner(this);
		}
		ASSERTNN(vbox)
		vbox->set_anchors_preset(godot::Control::LayoutPreset::PRESET_FULL_RECT);
		vbox->set_h_size_flags(godot::Control::SizeFlags::SIZE_EXPAND_FILL);
		{ // tmp: reallocate all buttons every time
			auto buttons = vbox->get_children();
			while (buttons.size()) {
				vbox->remove_child(cast_to<Node>(buttons.pop_back()));
			}
		}

		auto dir = godot::DirAccess::open(levelspath);
		ASSERT(dir.is_valid())

		auto files = dir->get_files();
		for (const auto& f : files) {
			auto* button = memnew(GuiButton);
			ASSERTNN(button)

			button->set_text(f);
			button->setOnPressed( // do we want to do scene transitions here or through some management system??
				[path = levelspath, tree = get_tree(), f]
				{
					LOG(WARN, "change_scene_to_file: ", path, f)
					tree->change_scene_to_file(godot::String(path) + f);
				});

			vbox->call_deferred("add_child", button);
			button->call_deferred("set_owner", vbox);
		}
	}
};
