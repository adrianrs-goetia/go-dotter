// #include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <character/camerapivot.h>
#include <character/playernode.h>
#include <mainnode.h>

#ifdef ENABLE_DD3D
#include <debugdraw3d/2d/config_2d.h>
#include <debugdraw3d/2d/debug_draw_2d.h>
#include <debugdraw3d/2d/graphs.h>
#include <debugdraw3d/2d/stats_2d.h>
#include <debugdraw3d/3d/config_3d.h>
#include <debugdraw3d/3d/config_scope_3d.h>
#include <debugdraw3d/3d/debug_draw_3d.h>
#include <debugdraw3d/3d/stats_3d.h>
#include <debugdraw3d/debug_draw_manager.h>
#include <debugdraw3d/utils/utils.h>
uint64_t debug_draw_manager_id = 0;
#endif

void init_gameplay_plugin_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) { return; }
	GDREGISTER_CLASS(MainNode)
	GDREGISTER_CLASS(CameraPivot)
	GDREGISTER_CLASS(PlayerNode)

	/// @DEBUGDRAW3D@ ///
#ifdef ENABLE_DD3D
	// TODO register as unexposed
	ClassDB::register_class<_DD3D_PhysicsWatcher>();
	ClassDB::register_class<_DD3D_WorldWatcher>();
	ClassDB::register_class<DebugDraw2D>();
	ClassDB::register_class<DebugDraw2DStats>();
	ClassDB::register_class<DebugDraw2DConfig>();
	ClassDB::register_class<DebugDraw2DGraph>();
	ClassDB::register_class<DebugDraw2DFPSGraph>();

	ClassDB::register_class<DebugDraw3D>();
	ClassDB::register_class<DebugDraw3DStats>();
	ClassDB::register_class<DebugDraw3DConfig>();
	ClassDB::register_class<DebugDraw3DScopeConfig>();

	ClassDB::register_class<DebugDrawManager>();

	// Since this manager is a node in the scene tree,
	// it will already be destroyed at the time of cleaning this library.
	DebugDrawManager* debug_draw_manager = memnew(DebugDrawManager);
	debug_draw_manager->init();
	debug_draw_manager_id = debug_draw_manager->get_instance_id();
#endif
}

void uninit_gameplay_plugin_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) { return; }

/// @DEBUGDRAW3D@ ///
// If this library is disabled manually before deleting the scene tree (hot-reload),
// then an attempt is made to delete this node manually.
#ifdef ENABLE_DD3D
	DebugDrawManager* debug_draw_manager =
			Object::cast_to<DebugDrawManager>(ObjectDB::get_instance(debug_draw_manager_id));
	if (Engine::get_singleton()->get_main_loop() && debug_draw_manager) { memdelete(debug_draw_manager); }
	debug_draw_manager_id = 0;
#endif
}

/// Entry point for plugin
// Name has to match ${PROJECT_NAME} from CMake configuration
// In the CMake the '-' dashes will be replaced with underscore to match this function name
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