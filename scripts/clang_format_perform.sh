#!/bin/bash

declare -a PATHS=("gameplay_cpp" "parameter_plugin/src")

for path in ${PATHS[@]}
do
    if find "$path" -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.cpp" \) | xargs clang-format -i; then
        echo "Successfully formatted code in $path"
    else
        echo "No files found to format in $path or an error occurred."
    fi
done

echo "clang-formatted code in paths: (${PATHS[*]})"
