#pragma once

#include <core/core.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include <algorithm>

class GamesceneHelperStairs : public godot::Node3D {
	GDCLASS(GamesceneHelperStairs, godot::Node3D)

	G_INT_IMPL(m_num, Num, = 3)
	G_FLOAT_IMPL(m_stepDepth, StepDepth, = -0.25f)
	G_FLOAT_IMPL(m_stepHeight, StepHeight, = 0.25f)
	G_FLOAT_IMPL(m_stepScale, StepScale, = 0.3f)
	G_FLOAT_IMPL(m_curveAngle, CurveAngle) // angle in degrees
	GS_PACKEDSCENE_IMPL(m_stairScene, StairScene)

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, Num, INT)
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, StepDepth, FLOAT)
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, StepHeight, FLOAT)
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, StepScale, FLOAT)
		METHOD_PROPERTY_IMPL(GamesceneHelperStairs, CurveAngle, FLOAT)
		METHOD_PROPERTY_PACKEDSCENE_IMPL(GamesceneHelperStairs, StairScene)
	}

	void _enter_tree() override {
		set_meta("_edit_group_", true);
		set_notify_transform(true);

		setAmountOfStairs();
		adjustStairTransform();
	}

	void _notification(int what) {
		switch (what) {
			case NOTIFICATION_TRANSFORM_CHANGED:
				adjustStairTransform();
				break;

			default:
				break;
		}
	}

	void setAmountOfStairs() {
		if (!m_stairScene.is_valid()) {
			LOG(ERROR, "Required scene not set for stairs")
			return;
		}
		ASSERT(m_stairScene->can_instantiate())

		// tmp, no need to delete all nodes every time
		auto children = get_children();
		while (children.size()) {
			auto* child = cast_to<Node>(children.pop_back());
			remove_child(child);
		}

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
	}

	void adjustStairTransform() {
		if (get_child_count() == 0) {
			LOG(DEBUG, "Tried adjusting stair positions without any child nodes")
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

	void setNum(int value) {
		m_num = std::max(value, 0);
		if (m_num > 0) {
			setAmountOfStairs();
			adjustStairTransform();
		}
	}

	void setStepDepth(float value) {
		m_stepDepth = value;
		adjustStairTransform();
	}

	void setStepHeight(float value) {
		m_stepHeight = value;
		adjustStairTransform();
	}

	void setStepScale(float value) {
		m_stepScale = value;
		adjustStairTransform();
	}

	void setCurveAngle(float value) {
		m_curveAngle = value;
		adjustStairTransform();
	}
};
