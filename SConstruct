#!/usr/bin/env python

from SCons.Script import SConscript
from SCons.Script.SConscript import SConsEnvironment

import SCons, SCons.Script
import shutil, os

import gameplay_cpp.sconstruct as gameplay_cpp
import godot_debug_draw_3d.sconstruct as dd3d

# current_dir = os.path.dirname(os.path.abspath(__file__))
current_dir = ""
project_name = "go-dotter"

env: SConsEnvironment = SConscript("godot-cpp/SConstruct")
env.CacheDir("build_cache")
env.Decider("MD5")
# env["LIBSPATH"] = "/home/ads/dev/go-dotter"
env.Append(CPPPATH=[])
env["STATIC_AND_SHARED_OBJECT_ARE_THE_SAME"] = 1

env_dd3d = env.Clone()
dd3d_libname = dd3d.configure_environment(env_dd3d, ARGUMENTS)

env_godotcpp = env_dd3d.Clone()
gameplay_cpp.configure_environment(env_godotcpp, [env.File(dd3d_libname)], ARGUMENTS)

# Copy .gdextension and .so .a files into project
copy_targets = [
    'godot_debug_draw_3d',
    'gameplay_cpp',
]
target_dir_base = os.path.join(current_dir, "project", "addons")
for target in copy_targets:
    target_dir = os.path.join(target_dir_base, "vendor")
    # if os.path.exists(target_dir):
    #     shutil.rmtree(target_dir)
    
    src_dir = os.path.join(target, "addons", target)
    for root, dirs, files in os.walk(src_dir):
        # Calculate the relative path from the source directory
        relative_path = os.path.relpath(root, src_dir)
        # Construct the target directory path
        target_path = os.path.join(target_dir, relative_path)
        
        # Create the target subdirectory
        os.makedirs(target_path, exist_ok=True)
        
        # Copy each file to the corresponding target directory
        for file_name in files:
            src_path = os.path.join(root, file_name)
            os.chmod(src_path, 0o0777)
            target_file_path = os.path.join(target_path, file_name)
            shutil.copy2(src_path, target_file_path)  # Copy and preserve metadata
