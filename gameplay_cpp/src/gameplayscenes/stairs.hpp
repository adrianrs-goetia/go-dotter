#pragma once

#include <core/core.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include <algorithm>

class GamesceneHelperStairs : public godot::Node3D {
	GDCLASS(GamesceneHelperStairs, godot::Node3D)

	G_PACKEDSCENE_IMPL(m_stairScene, StairScene)
	G_INT_IMPL(m_num, Num, = 3)
	G_FLOAT_IMPL(m_stepDepth, StepDepth, = -0.25f)
	G_FLOAT_IMPL(m_stepHeight, StepHeight, = 0.25f)
	G_FLOAT_IMPL(m_stepScale, StepScale, = 0.3f)
	G_FLOAT_IMPL(m_curveAngle, CurveAngle, = 0.f) // angle in degrees

	// Helper for allocations, deallocations and transform adjustments without errors
	enum {
		NONE,
		ALLOCATING,
		DEALLOCATING,
	} state;

public:
	static void _bind_methods() {
		METHOD_PROPERTY_PACKEDSCENE_IMPL(GamesceneHelperStairs, StairScene)
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, Num, INT)
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, StepDepth, FLOAT)
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, StepHeight, FLOAT)
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, StepScale, FLOAT)
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, CurveAngle, FLOAT)
	}

	void _enter_tree() override {
		set_meta("_edit_group_", true);
		set_notify_transform(true);

		/**
		 * Only allocate children here
		 * When adjusting transform, we require use of global transforms. However, getting global
		 * transforms before a node has entered the tree returns an error. As the child node has not
		 * yet been properly instantiated and entered the node tree.
		 */
		{
			state = ALLOCATING;
			set_process(true);
			set_process_internal(true);
		}
		if (godot::Engine::get_singleton()->is_editor_hint()) {
			if (m_stairScene.is_valid() && is_node_ready()) {
				allocateChildren();
			}
		}
		else {
			allocateChildren();
			adjustChildTransforms();
		}
	}

	void _process(double delta) override {
		switch (state) {
			case ALLOCATING: {
				bool ready = true;
				auto children = get_children();
				for (int i = 0; i < children.size(); i++) {
					auto child = cast_to<Node>(children.pop_back());
					if (!child->is_inside_tree() || !child->is_node_ready()) {
						ready = false;
						break;
					}
				}
				if (ready) {
					state = NONE;
					set_process(false);
					set_process_internal(false);
					adjustChildTransforms();
				}
				break;
			}
			case DEALLOCATING: {
				if (get_children().is_empty()) {
					{
						state = ALLOCATING;
					}
					allocateChildren();
				}
				break;
			}

			default:
				break;
		}
	}

	void _notification(int what) {
		switch (what) {
			case NOTIFICATION_TRANSFORM_CHANGED: {
				if (is_node_ready())
					adjustChildTransforms();
				break;
			}
			case ENotifications::INTERNAL_RELOAD: {
				if (is_node_ready()) {
					LOG(DEBUG, "Internal reload:", get_name())
					resetAll();
				}
				break;
			}

			default:
				break;
		}
	}

	void allocateChildren() {
		if (m_stairScene.is_null()) {
			LOG(ERROR, "Required scene not set for stairs")
			return;
		}
		ASSERT(m_stairScene->can_instantiate())

		auto children = get_children();

		while (getNum() <= children.size()) {
			auto* child = cast_to<Node>(children.pop_back());
			remove_child(child);
		}

		if (children.size() < getNum()) {
			int toAdd = getNum() - (int)children.size();
			Node* step = nullptr;

			if (children.size() == 0) {
				step = m_stairScene->instantiate();
				ASSERTNN(step)
				add_child(step);
				toAdd--;
			}
			else {
				step = cast_to<Node>(children.pop_back());
			}

			for (int i = 0; i < toAdd; i++) {
				auto* nextStep = m_stairScene->instantiate();
				ASSERTNN(step)
				step->add_sibling(nextStep);
				step = nextStep;
			}
		}
	}

	void resetAll() {
		removeAll();
		{
			state = DEALLOCATING;
			set_process(true);
			set_process_internal(true);
		}
	}

	void removeAll() {
		auto children = get_children();
		while (children.size()) {
			auto* child = cast_to<Node>(children.pop_back());
			call_deferred("remove_child", (child));
		}
	}

	void adjustChildTransforms() {
		if (get_child_count() == 0) {
			return;
		}

		const float anglePerStep = get_child_count() > 1 ? getCurveAngle() / (float)(get_child_count() - 1) : 0.f;
		auto pos = get_global_position();
		auto forward = get_global_basis().get_column(2);
		auto up = get_global_basis().get_column(1);

		auto children = get_children();

		// First step has transform equal to this
		{
			auto* child = cast_to<Node3D>(children.pop_front());
			ASSERTNN(child)
			child->set_global_position(pos);
			child->set_scale(godot::Vector3(1, 1, 1) * getStepScale());
		}

		while (children.size()) {
			auto* child = cast_to<Node3D>(children.pop_front());
			ASSERTNN(child)

			forward.rotate(up, godot::Math::deg_to_rad(anglePerStep));
			pos += (forward.normalized() * getStepDepth()) + (up * getStepHeight());
			child->set_global_position(pos);

			child->set_global_basis(godot::Basis(up.cross(forward).normalized(), up, forward));

			child->set_scale(godot::Vector3(1, 1, 1) * getStepScale());
		}
	}

	void setStairScene(godot::Ref<godot::PackedScene> value) {
		m_stairScene = value;

		if (m_stairScene.is_null()) {
			removeAll();
			return;
		}

		if (is_node_ready())
			resetAll();
	}

	void setNum(int value) {
		m_num = std::max(value, 1);
		if (m_num > 0 && is_node_ready()) {
			allocateChildren();
			adjustChildTransforms();
		}
	}

	void setStepDepth(float value) {
		m_stepDepth = value;

		if (is_node_ready())
			adjustChildTransforms();
	}

	void setStepHeight(float value) {
		m_stepHeight = value;
		if (is_node_ready())
			adjustChildTransforms();
	}

	void setStepScale(float value) {
		m_stepScale = value;
		if (is_node_ready())
			adjustChildTransforms();
	}

	void setCurveAngle(float value) {
		m_curveAngle = value;
		if (is_node_ready())
			adjustChildTransforms();
	}
};
