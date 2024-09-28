#!/bin/bash -e

CWD="$(pwd)"

ROOT="$(git rev-parse --show-toplevel)"
cd $ROOT/godot_debug_draw_3d
echo "DIR: $(pwd)"

git apply --ignore-space-change --ignore-whitespace $ROOT/patches/godot_debug_draw_3d/godot-cpp.patch
git apply --ignore-space-change --ignore-whitespace $ROOT/patches/godot_debug_draw_3d/sconstruct.patch
git apply --ignore-space-change --ignore-whitespace $ROOT/patches/godot_debug_draw_3d/plugin-cpp-api.patch

scons

cd $CWD