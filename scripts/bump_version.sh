#!/bin/sh -e

if [ -z "$1" ]; then
    echo "Missing argument <major|minor|patch>"
    return 1
fi

ROOT=$(git rev-parse --show-toplevel)
BUMP=$(echo "$1" | tr '[:lower:]' '[:upper:]')

if [ -z "$2" ]; then
    echo "Missing tag message as second argument"
    return 2
fi
TAG_MESSAGE="$2"

if [ ! "$BUMP" = "MAJOR" ] && [ ! "$BUMP" = "MINOR" ] && [ ! "$BUMP" = "PATCH" ]; then
    echo "Incorrect argument. Expected <major|minor|patch>"
    return 2
fi

# Pattern match to get value to bump
VERSION_FILE="gameplay_cpp/version.h"
PATTERN="#define GODOTTER_${BUMP}"
BUMP_VALUE=$(grep -oP "$PATTERN \K\d+" "$VERSION_FILE")
if [ -z "$BUMP_VALUE" ]; then
    echo "Failed to find bump=$BUMP for pattern=$PATTERN"
    return 3
fi
BUMP_VALUE=$((BUMP_VALUE+1))

# Get current version number
MAJOR=$(grep -oP '#define GODOTTER_MAJOR \K\d+' "$VERSION_FILE")
MINOR=$(grep -oP '#define GODOTTER_MINOR \K\d+' "$VERSION_FILE")
PATCH=$(grep -oP '#define GODOTTER_PATCH \K\d+' "$VERSION_FILE")
CURRENT_VERSION="${MAJOR}.${MINOR}.${PATCH}"

echo "current version == ${CURRENT_VERSION}"

# Bump value
sed -i "s|^${PATTERN}.*|${PATTERN} ${BUMP_VALUE}|" "$VERSION_FILE"

# If bump major or minor, change lower values to 0
if [ "$BUMP" = "MAJOR" ]; then
    sed -i "s|^#define GODOTTER_MINOR.*|#define GODOTTER_MINOR 0|" "$VERSION_FILE"
    sed -i "s|^#define GODOTTER_PATCH.*|#define GODOTTER_PATCH 0|" "$VERSION_FILE"
elif [ "$BUMP" = "MINOR" ];then
    sed -i "s|^#define GODOTTER_PATCH.*|#define GODOTTER_PATCH 0|" "$VERSION_FILE"
fi

# Read value from file
MAJOR=$(grep -oP '#define GODOTTER_MAJOR \K\d+' "$VERSION_FILE")
MINOR=$(grep -oP '#define GODOTTER_MINOR \K\d+' "$VERSION_FILE")
PATCH=$(grep -oP '#define GODOTTER_PATCH \K\d+' "$VERSION_FILE")
VERSION="${MAJOR}.${MINOR}.${PATCH}"

# Overwrite .gdextension version
GDEXTENSION_FILE="gameplay_cpp/addons/gameplay_cpp/gameplaycpp.gdextension"
sed -i "s|${CURRENT_VERSION}|${VERSION}|" "$GDEXTENSION_FILE"

git add \
    $VERSION_FILE \
    $GDEXTENSION_FILE
git commit -m "Version $VERSION, $TAG_MESSAGE"

if [ -n "$TAG_MESSAGE" ]; then
    git tag -a "v$VERSION" -m "$TAG_MESSAGE"
else
    git tag "v$VERSION"
fi


echo "New version $VERSION"