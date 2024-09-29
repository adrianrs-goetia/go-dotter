#!/bin/bash -e

# !NB: Run this after setup_environment.sh

# Run script in project root
# Cannot be called from a submodule
ROOT="$(git rev-parse --show-toplevel)"
cd $ROOT

## Apply special git patches
# 
cd $ROOT/godot_debug_draw_3d
git submodule deinit
git apply --ignore-space-change --ignore-whitespace ../patches/godot_debug_draw_3d/big_patch_making_dd3d_submodule_to_godotter.patch
cd $ROOT
#
cd $ROOT/godot-cpp
git apply --ignore-space-change --ignore-whitespace ../patches/godot-cpp/no_default_target.patch
cd $ROOT

# Run scons
scons target=editor
scons target=template_debug