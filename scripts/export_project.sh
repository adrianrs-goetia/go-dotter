#!/bin/sh -e

# NB!
# Before exporting the project. Make sure that it has been opened at least once in the editor.
# This is due to Blender files not having been loaded and the project lacking some metadata

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Missing arguments. <path_to_godot_executable> <debug|release>"
    return 1
fi
GODOT_EXECUTABLE=$1
MODE=$(echo "$2" | tr '[:upper:]' '[:lower:]')

# Build template version of code
scons install_mode=all
scons target=template_$MODE install_mode=all

# Get version
VERSION_FILE="gameplay_cpp/version.h"
MAJOR=$(grep -oP '#define GODOTTER_MAJOR \K\d+' "$VERSION_FILE")
MINOR=$(grep -oP '#define GODOTTER_MINOR \K\d+' "$VERSION_FILE")
PATCH=$(grep -oP '#define GODOTTER_PATCH \K\d+' "$VERSION_FILE")
VERSION="${MAJOR}.${MINOR}.${PATCH}"

EXPORT_VERSION="v${VERSION}_${MODE}"
mkdir -p build/export/${EXPORT_VERSION}
cp project/config.json build/export/${EXPORT_VERSION}/

# Configure export preset
EXPORT_PATH="export_path=\"../build/export/${EXPORT_VERSION}/go-dotter.${EXPORT_VERSION}\""
EXPORT_CONFIG_FILE="project/export_presets.cfg"
sed -i "s|^export_path=.*|${EXPORT_PATH}|" "$EXPORT_CONFIG_FILE"

# Export project
cd project
$GODOT_EXECUTABLE --headless --export-$MODE "Linux/X11"
cd -