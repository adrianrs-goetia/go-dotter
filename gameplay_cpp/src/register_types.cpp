#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <character/cameraPivot.h>
#include <character/playerNode.h>
#include <components/grappleInstigatorComponent.h>
#include <components/grappleTargetComponent.h>
#include <components/inputComponent.h>
#include <components/parryInstigatorComponent.h>
#include <components/parryTargetComponent.h>
#include <mainNode.h>
#include <npcs/projectile/projectile.h>
#include <npcs/turret/turretNode.h>
#include <core/nodeComponent.hpp>

using namespace godot;

void init_gameplay_plugin_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) { return; }
	godot::ClassDB::register_class<MainNode>();
	godot::ClassDB::register_class<CameraPivot>();
	godot::ClassDB::register_class<PlayerNode>();

	// Components
	godot::ClassDB::register_class<InputComponent>();
	godot::ClassDB::register_abstract_class<NodeComponent>();
	godot::ClassDB::register_class<GrappleInstigatorComponent>();
	godot::ClassDB::register_class<GrappleTargetComponent>();
	godot::ClassDB::register_class<ParryInstigatorComponent>();
	godot::ClassDB::register_class<ParryTargetComponent>();

	// npcs
	godot::ClassDB::register_class<TurretNode>();
	godot::ClassDB::register_class<Projectile>();
}

void uninit_gameplay_plugin_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) { return; }
}

/// Entry point for plugin
extern "C" {
GDExtensionBool GDE_EXPORT gameplay_plugin_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
		const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(init_gameplay_plugin_module);
	init_obj.register_terminator(uninit_gameplay_plugin_module);
	init_obj.set_minimum_library_initialization_level(
			godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}