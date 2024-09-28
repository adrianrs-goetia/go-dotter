#!/bin/bash -e

CWD="$(pwd)"

ROOT="$(git rev-parse --show-toplevel)"
cd $ROOT/godot_debug_draw_3d

# Reset to origin of branch and then apply custom patches on top
git reset --hard
git apply --ignore-space-change --ignore-whitespace $ROOT/patches/godot_debug_draw_3d/godot-cpp.patch
git apply --ignore-space-change --ignore-whitespace $ROOT/patches/godot_debug_draw_3d/sconstruct.patch
git apply --ignore-space-change --ignore-whitespace $ROOT/patches/godot_debug_draw_3d/plugin-cpp-api.patch

scons

# godot_debug_draw_3d is assumed to be on level below go-dotter project root
cd ..
if [ ! -d project/addons/debug_draw_3d ]; then
    ln -sr godot_debug_draw_3d/addons/debug_draw_3d/ project/addons/debug_draw_3d
fi

cd $CWD