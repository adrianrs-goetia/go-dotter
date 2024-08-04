#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "src/node3d_extension.h""

void init_trialplugin_module(godot::ModuleInitializationLevel p_level)
{
    if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE){
        return;
    }

    GDREGISTER_CLASS(godot::Node3DExtension)
}

void uninit_trialplugin_module(godot::ModuleInitializationLevel p_level)
{
    if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE){
        return;
    }
}

extern "C" {

GDExtensionBool GDE_EXPORT example_module_init(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    const GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization* r_initialization
){
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(init_trialplugin_module);
    init_obj.register_terminator(uninit_trialplugin_module);
    init_obj.set_minimum_library_initialization_level(godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}

}