#include "parrytargetcomponent.h"

#include <godot_cpp/classes/collision_shape3d.hpp>

void ParryTargetComponent::_bind_methods() {
	DEFAULT_PROPERTY(ParryTargetComponent)
	//

	godot::ClassDB::bind_method(D_METHOD("getAreaPath"), &ParryTargetComponent::getAreaPath);
	godot::ClassDB::bind_method(D_METHOD("setAreaPath", "path"), &ParryTargetComponent::setAreaPath);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "AreaPath"), "setAreaPath", "getAreaPath");
}

void ParryTargetComponent::_enter_tree() {
	m_areaPtr = get_node<Area3D>(m_pathToArea3D);
	if (!m_areaPtr) {
		String msg;
		msg += get_name();
		msg += ", failed to fetch m_areaPtr for collision. Please assign a path to a CollisionShape3D to 'Area Path'";
		ERR_PRINT_ONCE(msg);
	}
	else { m_areaPtr->set_collision_layer_value(collisionflags::parrydetection, true); }

	RETURN_IF_EDITOR
	ASSERT_NOTNULL(m_areaPtr)
}

void ParryTargetComponent::_exit_tree() {
	if (m_areaPtr) { m_areaPtr->set_collision_layer_value(collisionflags::parrydetection, true); }
	m_areaPtr = nullptr;
}

void ParryTargetComponent::setAreaPath(NodePath path) { m_pathToArea3D = path; }

NodePath ParryTargetComponent::getAreaPath() { return m_pathToArea3D; }
