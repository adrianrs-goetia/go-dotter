#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "configHandler.h"

void init_parameter_plugin_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	godot::ClassDB::register_class<ConfigHandler>();
}

void uninit_parameter_plugin_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

/// Entry point for plugin
extern "C" {
GDExtensionBool GDE_EXPORT parameter_plugin_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
		const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(init_parameter_plugin_module);
	init_obj.register_terminator(uninit_parameter_plugin_module);
	init_obj.set_minimum_library_initialization_level(
			godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}