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
	G_FLOAT_IMPL(m_curveAngle, CurveAngle) // angle in degrees

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

		if (m_stairScene.is_valid()) {
			allocateAmount();
			adjustTransform();
		}
	}

	void _notification(int what) {
		switch (what) {
			case NOTIFICATION_TRANSFORM_CHANGED:
				adjustTransform();
				break;

			default:
				break;
		}
	}

	void allocateAmount() {
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
				ASSERT_NOTNULL(step)
				add_child(step);
				toAdd--;
			}
			else {
				step = cast_to<Node>(children.pop_back());
			}

			for (int i = 0; i < toAdd; i++) {
				auto* nextStep = m_stairScene->instantiate();
				ASSERT_NOTNULL(step)
				step->add_sibling(nextStep);
				step = nextStep;
			}
		}
	}

	void resetAll() {
		removeAll();

		ASSERT(m_stairScene.is_valid())
		Node* step = nullptr;
		step = m_stairScene->instantiate();
		ASSERT_NOTNULL(step)
		add_child(step);
		for (int i = 1; i < m_num; i++) {
			auto* nextStep = m_stairScene->instantiate();
			ASSERT_NOTNULL(step)
			step->add_sibling(nextStep);
			step = nextStep;
		}
		adjustTransform();
	}

	void removeAll() {
		auto children = get_children();
		while (children.size()) {
			auto* child = cast_to<Node>(children.pop_back());
			remove_child(child);
		}
	}

	void adjustTransform() {
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
			ASSERT_NOTNULL(child)
			child->set_global_position(pos);
			child->set_scale(godot::Vector3(1, 1, 1) * getStepScale());
		}

		while (children.size()) {
			auto* child = cast_to<Node3D>(children.pop_front());
			ASSERT_NOTNULL(child)

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
			allocateAmount();
			adjustTransform();
		}
	}

	void setStepDepth(float value) {
		m_stepDepth = value;

		if (is_node_ready())
			adjustTransform();
	}

	void setStepHeight(float value) {
		m_stepHeight = value;
		if (is_node_ready())
			adjustTransform();
	}

	void setStepScale(float value) {
		m_stepScale = value;
		if (is_node_ready())
			adjustTransform();
	}

	void setCurveAngle(float value) {
		m_curveAngle = value;
		if (is_node_ready())
			adjustTransform();
	}
};
