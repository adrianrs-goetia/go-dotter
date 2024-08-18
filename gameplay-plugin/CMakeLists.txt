project(gameplay-plugin CXX)

option(EXPORT "Exporting game for publishing" OFF)

# Source files
set(SOURCES

    src/camerapivot.cpp
    src/core.cpp
    src/mainnode.cpp
    src/playerfsm.cpp
    src/playernode.cpp
    src/playerstates.cpp
    # src/_translation_unit.cpp
)

# Shared library
add_library(
    ${PROJECT_NAME}
    SHARED

    register_types.cpp # Always added
    ${SOURCES}
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE godot-cpp
)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_definitions(${PROJECT_NAME} PRIVATE HOT_RELOAD_ENABLED)

# GDExtension file generation from gdextension.in
string(TOLOWER "${PROJECT_NAME}.gdextension" GDEXTENSION_CONFIGURATION_FILE)
string(TOLOWER ${PROJECT_NAME} GDEXTENSION_ENTRY_POINT)
string(REGEX REPLACE "-" "_" GDEXTENSION_ENTRY_POINT ${GDEXTENSION_ENTRY_POINT})
if (EXPORT)
    set(DLLPATH "")
else()
    set(DLLPATH "${CMAKE_CURRENT_BINARY_DIR}/bin/")
endif()
configure_file(gdextension.in ${GDEXTENSION_CONFIGURATION_FILE})

# Build file locations
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/bin>
    LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/bin>
    RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/bin>
)

# Installation of .gdextension and .dll file into project
# .dll is installed but not used during development.
# Manually reinstall when exporting the project
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${GDEXTENSION_CONFIGURATION_FILE}"
    CONFIGURATIONS Release Debug
    DESTINATION ${CMAKE_HOME_DIRECTORY}/project/bin/${PROJECT_NAME}
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/bin/${PROJECT_NAME}.dll"
    CONFIGURATIONS Release Debug
    DESTINATION ${CMAKE_HOME_DIRECTORY}/project/bin/${PROJECT_NAME}
)