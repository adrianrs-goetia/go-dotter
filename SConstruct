#!/usr/bin/env python

from SCons.Script import SConscript, Variables, EnumVariable
from SCons.Script.SConscript import SConsEnvironment

import SCons, SCons.Script
import shutil, os

import gameplay_cpp.sconstruct as gameplay_cpp
import godot_debug_draw_3d.sconstruct as dd3d
import parameter_plugin.sconstruct as paramplug

from scripts.generate_config_hpp import generate_hpp_from_json

project_name = "go-dotter"

env: SConsEnvironment = SConscript("godot-cpp/SConstruct")
env.CacheDir("build/cache")
env["build_dir"] = "build"
env["obj_dir"] = "build/obj"
os.makedirs("build/export", exist_ok=True)
env.Decider("MD5")
env.Append(CPPPATH=['libraries/'])
env.Append(LINKFLAGS=['-Wl,-z,defs'])
env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1
if env["target"] == "template_debug":
        env.Append(CPPDEFINES=["TESTS_ENABLED"])
env.Append( CPPPATH=['Godot-SFT'] )

opts = Variables()
opts.Add(
    EnumVariable(
        key="install_mode",
        help="Desired install mode. By default builds and install all targets. Mode 'gameplay'"
            "only installs plugins that are hot reloadable",
        default=env.get("install_mode", "all"),
        allowed_values=("all", "gameplay")
    )
)
opts.Update(env)

# Generate code from json configs
generate_hpp_from_json("project/config.json", "parameter_plugin/generated/configtypes.hpp")
generate_hpp_from_json("project/application.json", "parameter_plugin/generated/applicationtypes.hpp")

# Section for creating and linking shared libraries
dd3d_libfile = dd3d.configure_environment(env, ARGUMENTS)
parameterplugin_libfile = paramplug.configure_environment(env, ARGUMENTS)
gameplay_cpp_libfile = gameplay_cpp.configure_environment(env, [dd3d_libfile, parameterplugin_libfile], ARGUMENTS)

# Move shared objects into correct dir after building is done
def post_build_action(target, source, env):
    vendor = "project/addons/vendor"
    libs = f"{vendor}/libs"
    os.makedirs(libs, exist_ok=True)
    with open(f"{libs}/.gdignore", "w"):
        pass

    install_mode = env.get("install_mode")
    if install_mode == "all":
        shutil.move(dd3d_libfile.name, f"{libs}/{dd3d_libfile.name}")
        os.chmod(f"{libs}/{dd3d_libfile.name}", 0o0777)
        shutil.copy2(f"godot_debug_draw_3d/addons/godot_debug_draw_3d/debug_draw_3d.gdextension", f"{vendor}/debug_draw_3d.gdextension")

        shutil.move(parameterplugin_libfile.name, f"{libs}/{parameterplugin_libfile.name}")
        os.chmod(f"{libs}/{parameterplugin_libfile.name}", 0o0777)
        shutil.copy2(f"parameter_plugin/addons/parameter_plugin/parameter_plugin.gdextension", f"{vendor}/parameter_plugin.gdextension")
        
    shutil.move(gameplay_cpp_libfile.name,  f"{libs}/{gameplay_cpp_libfile.name}")
    # Godot might not be able to load shared object without correct permission. This is a lazy fix
    os.chmod(f"{libs}/{gameplay_cpp_libfile.name}", 0o0777)
    shutil.copy2(f"gameplay_cpp/addons/gameplay_cpp/gameplaycpp.gdextension", f"{vendor}/gameplaycpp.gdextension")

env.AddPostAction(gameplay_cpp_libfile, post_build_action)
