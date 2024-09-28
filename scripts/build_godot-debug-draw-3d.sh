#!/bin/bash -e

# Expected scons targets to either be ('editor', 'template_release', 'template_debug')
# by default use 'editor', but use template_* when exporting project
if [ -z "$1" ]; then
    echo "ERROR"
    echo "Missing argument for scons target, expected to either be (\'editor\', \'template_release\', \'template_debug\')"
    echo "e.g. target=editor"
    exit 1
fi

CWD="$(pwd)"

ROOT="$(git rev-parse --show-toplevel)"
cd $ROOT/godot_debug_draw_3d

# Reset to origin of branch and then apply custom patches on top
git reset --hard
git apply --ignore-space-change --ignore-whitespace $ROOT/patches/godot_debug_draw_3d/godot-cpp.patch
git apply --ignore-space-change --ignore-whitespace $ROOT/patches/godot_debug_draw_3d/sconstruct.patch
git apply --ignore-space-change --ignore-whitespace $ROOT/patches/godot_debug_draw_3d/plugin-cpp-api.patch

mkdir -p $ROOT/obj
export SCONS_CACHE="$ROOT/obj"
scons $1

# is assumed to be on one level below go-dotter project root
cd ..
mkdir -p project/addons/debug_draw_3d
touch godot_debug_draw_3d/addons/libs/.gdignore
cp -rf godot_debug_draw_3d/addons/debug_draw_3d/* project/addons/debug_draw_3d/
chmod +r project/addons/debug_draw_3d/*

cd $CWD