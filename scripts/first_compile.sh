#!/bin/bash -e

# !NB: Run this after setup_environment.sh
if [ -z "$1" ]; then
    echo "To avoid accidentaly running this script, enter a dummy value as an argument"
    exit 1
fi
echo "Running the first compile of go-dotter"

# Run script in project root
# Cannot be called from a submodule
ROOT="$(git rev-parse --show-toplevel)"
cd $ROOT

# Run scons
## dev_build=yes ensures debug symbols are added for breakpoints
scons target=editor install_mode=all dev_build=yes
scons target=template_debug