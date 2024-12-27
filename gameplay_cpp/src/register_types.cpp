#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <character/cameraPivot.h>
#include <character/playerNode.h>
#include <character/playerGui.hpp>

#include <gameplayscenes/billboard.hpp>
#include <gameplayscenes/stairs.hpp>

#include <components/animation.hpp>
#include <components/attackInstigator.hpp>
#include <components/attackTarget.hpp>
#include <components/grappleInstigator.hpp>
#include <components/grappleTarget.hpp>
#include <components/parryInstigator.hpp>
#include <components/parryTarget.hpp>

#include <mainNode.h>
#include <managers/inputManager.h>

#include <npcs/grapplenode.h>
#include <npcs/projectile/projectile.h>
#include <npcs/turret/turretNode.h>

#include <../tests/test_circularbuffer.h>
#include <components/baseClasses/grappleBase.hpp>
#include <core/nodeComponent.hpp>

using namespace godot;

void init_parameter_plugin_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	godot::ClassDB::register_class<CameraPivot>();
	godot::ClassDB::register_class<MainNode>();
	godot::ClassDB::register_class<PlayerNode>();
	godot::ClassDB::register_class<PlayerGui>();

	// Managers
	godot::ClassDB::register_class<InputManager>();

	// Components
	godot::ClassDB::register_abstract_class<NodeComponent>();
	godot::ClassDB::register_abstract_class<ComponentGrappleBase>();

	godot::ClassDB::register_class<ComponentAttackInstigator>();
	godot::ClassDB::register_class<ComponentAttackTarget>();
	godot::ClassDB::register_class<ComponentAnimation>();
	godot::ClassDB::register_class<ComponentGrappleInstigator>();
	godot::ClassDB::register_class<ComponentGrappleTarget>();
	godot::ClassDB::register_class<ComponentParryInstigator>();
	godot::ClassDB::register_class<ComponentParryTarget>();

	// npcs
	godot::ClassDB::register_class<GrappleNode>();
	godot::ClassDB::register_class<Projectile>();
	godot::ClassDB::register_class<TurretNode>();

	// gameplayscenes
	godot::ClassDB::register_class<GamesceneHelperStairs>();
	godot::ClassDB::register_class<GamesceneBillboard>();

// do tests if in testing mode
#ifdef TESTS_ENABLED
	do_tests();
#endif
}

void uninit_parameter_plugin_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

/// Entry point for plugin
extern "C" {
GDExtensionBool GDE_EXPORT gameplay_plugin_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
	const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(init_parameter_plugin_module);
	init_obj.register_terminator(uninit_parameter_plugin_module);
	init_obj.set_minimum_library_initialization_level(
		godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}