#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <optional>

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/bone_attachment3d.hpp>
#include <godot_cpp/classes/geometry_instance3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/skeleton3d.hpp>

#include <configparams.hpp>
#include <utils/animationTweener.hpp>

class ComponentWeapon : public godot::BoneAttachment3D {
	GDCLASS(ComponentWeapon, godot::BoneAttachment3D)
public:
	/***********
	 * BINDING *
	 **********/
	NodePath m_pathWielderNode;
	NodePath m_pathTrailNode;
	NodePath m_pathWeaponNode;
	std::optional<GPUParticles3D*> m_trail = nullptr;
	std::optional<Node*> m_wielder = nullptr;
	std::optional<Node*> m_weapon = nullptr;

	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(ComponentWeapon, RootWielderNode, NODE_PATH);
		METHOD_PROPERTY_IMPL(ComponentWeapon, WeaponNode, NODE_PATH);
		METHOD_PROPERTY_IMPL(ComponentWeapon, TrailNode, NODE_PATH);
	} 

	void     setRootWielderNode(NodePath path) { m_pathWielderNode = path; }
	NodePath getRootWielderNode() const        { return m_pathWielderNode; }
	void     setWeaponNode(NodePath path)      { m_pathWeaponNode = path; }
	NodePath getWeaponNode() const             { return m_pathWeaponNode; }	
	void     setTrailNode(NodePath path)       { m_pathTrailNode = path; }
	NodePath getTrailNode() const              { return m_pathTrailNode; }		

	/***************
	 * GODOT SETUP *
	 **************/


	void _enter_tree() override {
		set_use_external_skeleton(true);
		if (!m_pathWielderNode.is_empty()) {
			m_wielder = get_node<godot::Node>(m_pathWielderNode);
			LOG(DEBUG, "GOT WEILDER IN ENTER TREE: ", m_pathWielderNode)
			auto* skelly = m_wielder.value()->get_node<godot::Skeleton3D>("Armature/Skeleton3D");
			set_external_skeleton(skelly->get_path());
			set_bone_idx(34);
		} else { LOG(DEBUG, "WIELDER PATH EMPTY ", m_pathWielderNode) }

		if (!m_pathWeaponNode.is_empty()) {
			LOG(DEBUG, "GOT WEAPON IN ENTER TREE: ", m_pathWeaponNode)
			m_weapon = get_node_internal(m_pathWeaponNode);
		} else { LOG(DEBUG, "WEAPON PATH EMPTY ", m_pathWeaponNode) }

		if (!m_pathTrailNode.is_empty()) {
			LOG(DEBUG, "GOT TRAIL IN ENTER TREE: ", m_pathTrailNode)
			m_trail = Object::cast_to<GPUParticles3D>(get_node_internal(m_pathTrailNode));
			m_trailTransform = m_trail.value()->get_transform();
			disableTrail(); 

		} else { LOG(DEBUG, "TRAIL PATH EMPTY ", m_pathTrailNode) }

		RETURN_IF_EDITOR(void())
	}

	void _process(double delta) override {
	}

	void _physics_process(double delta) override {
		RETURN_IF_EDITOR(void())
	}

	/**********
	 * CUSTOM *
	 **********/

	godot::Transform3D m_trailTransform;


	void enableTrail() {
		//a little hacky
		if (m_trail.has_value()) { // *sigh* I miss monads
			m_trail.value()->set_as_top_level(false);
			m_trail.value()->set_transform(m_trailTransform);
		} else {
			LOG(DEBUG, "Tried enabling trail when trail object not set")
		}
	}

	void disableTrail() {
		//a little hacky
		if (m_trail.has_value()) { // *sigh* I miss monads
			//store transform, since toplevel breaks it
			m_trailTransform = m_trail.value()->get_transform();
			m_trail.value()->set_as_top_level(true);
		} else {
			LOG(DEBUG, "Tried disabling trail when trail object not set")
		}
	}
};
