#pragma once

#define GODOTTER_MAJOR 0
#define GODOTTER_MINOR 3
#define GODOTTER_PATCH 23
#define GODOTTER_VERSION ((GODOTTER_MAJOR << (8 * 3)) + (GODOTTER_MINOR << (8 * 2)) + (GODOTTER_PATCH << (8 * 1)))

#define _GODOTTER_VERSION_STR_TEXT(text) #text
#define _GODOTTER_VERSION_STR(major, minor, patch)                                                                     \
	_GODOTTER_VERSION_STR_TEXT(major) "." _GODOTTER_VERSION_STR_TEXT(minor) "." _GODOTTER_VERSION_STR_TEXT(patch)
#define GODOTTER_VERSION_STR _GODOTTER_VERSION_STR(GODOTTER_MAJOR, GODOTTER_MINOR, GODOTTER_PATCH)
