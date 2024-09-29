#!/usr/bin/env python

from SCons.Script import SConscript
from SCons.Script.SConscript import SConsEnvironment

import SCons, SCons.Script
import shutil, os

import gameplay_cpp.sconstruct as gameplay_cpp
import godot_debug_draw_3d.sconstruct as dd3d

project_name = "go-dotter"

env: SConsEnvironment = SConscript("godot-cpp/SConstruct")
env.CacheDir("bin/cache")
env.Decider("MD5")
env.Append(CPPPATH=[])
env.Append(LINKFLAGS=['-Wl,-z,defs'])

# Section for creating and linking shared libraries
dd3d_libfile = dd3d.configure_environment(env, ARGUMENTS)
gameplay_cpp_libfile = gameplay_cpp.configure_environment(env, [dd3d_libfile], ARGUMENTS)

# Move shared objects into correct dir after building is done
def post_build_action(target, source, env):
    vendor = "project/addons/vendor"
    libs = f"{vendor}/libs"
    os.makedirs(libs, exist_ok=True)
    with open(f"{libs}/.gdignore", "w"):
        pass

    shutil.move(gameplay_cpp_libfile.name,  f"{libs}/{gameplay_cpp_libfile.name}")
    shutil.move(dd3d_libfile.name,          f"{libs}/{dd3d_libfile.name}")
    # Godot might not be able to load shared object without correct permission. This is a lazy fix
    os.chmod(f"{libs}/{gameplay_cpp_libfile.name}", 0o0777)
    os.chmod(f"{libs}/{dd3d_libfile.name}",         0o0777)
    shutil.copy2(f"godot_debug_draw_3d/addons/godot_debug_draw_3d/debug_draw_3d.gdextension",   f"{vendor}/debug_draw_3d.gdextension")
    shutil.copy2(f"gameplay_cpp/addons/gameplay_cpp/gameplaycpp.gdextension",                   f"{vendor}/gameplaycpp.gdextension")

env.AddPostAction(gameplay_cpp_libfile, post_build_action)
