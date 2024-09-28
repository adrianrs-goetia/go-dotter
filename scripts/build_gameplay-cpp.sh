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
cd $ROOT/gameplay_cpp

mkdir -p $ROOT/obj
export SCONS_CACHE="$ROOT/obj"
scons $1

# is assumed to be on level below go-dotter project root
cd ..
mkdir -p project/addons/gameplay_cpp
cp -rf gameplay_cpp/addons/gameplay_cpp/* project/addons/gameplay_cpp/
chmod +r project/addons/gameplay_cpp/*

cd $CWD