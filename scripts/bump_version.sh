#!/bin/sh -e

if [ -z "$1" ]; then
    echo "Missing argument <major|minor|patch>"
    return 1
fi

BUMP=$(echo "$1" | tr '[:lower:]' '[:upper:]')

TAG_MESSAGE=""
if [ -n "$2" ]; then
    TAG_MESSAGE="$2"
else
    echo "No tag message added. Using latest commit message."
fi

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

# Bump value
sed -i "s|^${PATTERN}.*|${PATTERN} ${BUMP_VALUE}|" "$VERSION_FILE"

# Read value from file
MAJOR=$(grep -oP '#define GODOTTER_MAJOR \K\d+' "$VERSION_FILE")
MINOR=$(grep -oP '#define GODOTTER_MINOR \K\d+' "$VERSION_FILE")
PATCH=$(grep -oP '#define GODOTTER_PATCH \K\d+' "$VERSION_FILE")
VERSION="${MAJOR}.${MINOR}.${PATCH}"

if [ -n "$TAG_MESSAGE" ]; then
    git tag -a "v$VERSION" -m "$TAG_MESSAGE"
else
    git tag "v$VERSION"
fi

echo "New version $VERSION"