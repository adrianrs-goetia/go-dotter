#pragma once

#include "typedefs.hpp"
#include <core/core.hpp>

#include <godot_cpp/classes/animation_node.hpp>
#include <godot_cpp/classes/animation_node_animation.hpp>
#include <godot_cpp/classes/animation_node_blend2.hpp>
#include <godot_cpp/classes/animation_node_blend_tree.hpp>
#include <godot_cpp/classes/animation_node_one_shot.hpp>
#include <godot_cpp/classes/animation_node_state_machine.hpp>
#include <godot_cpp/classes/animation_node_sync.hpp>
#include <godot_cpp/classes/animation_root_node.hpp>

class GodotterAnimationNodeOneShot : public godot::AnimationNodeOneShot {
	GDTYPE(GodotterAnimationNodeOneShot, godot::AnimationNodeOneShot)

public:
	enum EBoneFilter {
		NONE,
		UPPERBODY,
		LOWERBODY
	};

	G_ENUM_IMPL(boneFilter, BoneFilter, EBoneFilter, = EBoneFilter::NONE)

public:
	static void _bind_methods() {
		METHOD_PROPERTY_ENUM_IMPL(GodotterAnimationNodeOneShot, BoneFilter, INT, "NONE,UPPERBODY,LOWERBODY")
	}

	void setBoneFilter(int val) {
		boneFilter = static_cast<EBoneFilter>(val);
		switch (boneFilter) {
			case EBoneFilter::NONE: {
				set_filter_enabled(false);
				for (auto&& c : bones::lowerBody) {
					set_filter_path(c, false);
				}
				for (auto&& c : bones::upperBody) {
					set_filter_path(c, false);
				}
				break;
			}
			case EBoneFilter::UPPERBODY: {
				set_filter_enabled(true);
				for (auto&& c : bones::upperBody) {
					set_filter_path(c, true);
				}
				for (auto&& c : bones::lowerBody) {
					set_filter_path(c, false);
				}
				break;
			}
			case EBoneFilter::LOWERBODY: {
				set_filter_enabled(true);
				for (auto&& c : bones::upperBody) {
					set_filter_path(c, false);
				}
				for (auto&& c : bones::lowerBody) {
					set_filter_path(c, true);
				}
				break;
			}

			default:
				break;
		}
	}

	GodotterAnimationNodeOneShot() {
		LOG(INFO, "OneShot constructed", get_name())
	}

	void _ready() {
		LOG(INFO, "OneShot _ready", get_name())
	}

	void _enter_tree() {
		LOG(INFO, "OneShot entered tree", get_name())
	}
};

VARIANT_ENUM_CAST(GodotterAnimationNodeOneShot::EBoneFilter);
