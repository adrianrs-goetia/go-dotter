project(debug-draw-3d-library CXX) # naming has to match the one in register_types.cpp

# Source files
set(SOURCES
    src/api.cpp
    src/debugdraw3d/2d/config_2d.cpp
    src/debugdraw3d/2d/debug_draw_2d.cpp
    src/debugdraw3d/2d/graphs.cpp
    src/debugdraw3d/2d/grouped_text.cpp
    src/debugdraw3d/2d/stats_2d.cpp
    src/debugdraw3d/3d/config_3d.cpp
    src/debugdraw3d/3d/config_scope_3d.cpp
    src/debugdraw3d/3d/debug_draw_3d.cpp
    src/debugdraw3d/3d/debug_geometry_container.cpp
    src/debugdraw3d/3d/geometry_generators.cpp
    src/debugdraw3d/3d/render_instances.cpp
    src/debugdraw3d/3d/stats_3d.cpp
    src/debugdraw3d/common/colors.cpp
    src/debugdraw3d/debug_draw_manager.cpp
    src/debugdraw3d/editor/asset_library_update_checker.cpp
    src/debugdraw3d/editor/editor_menu_extensions.cpp
    src/debugdraw3d/editor/generate_csharp_bindings.cpp
    src/debugdraw3d/gen/editor_resources.gen.cpp
    src/debugdraw3d/gen/shared_resources.gen.cpp
    src/debugdraw3d/utils/math_utils.cpp
    src/debugdraw3d/utils/utils.cpp
)

add_library(
    ${PROJECT_NAME}
    SHARED
    register_types.cpp
    ${SOURCES}
)
target_link_libraries(${PROJECT_NAME} PRIVATE godot-cpp)
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/debugdraw3d
)

# Api library
add_library(debugdraw3d_api INTERFACE)
target_include_directories(debugdraw3d_api INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# GDExtension file generation from gdextension.in
string(TOLOWER "${PROJECT_NAME}.gdextension" GDEXTENSION_CONFIGURATION_FILE)
string(TOLOWER ${PROJECT_NAME} GDEXTENSION_ENTRY_POINT)
string(REGEX REPLACE "-" "_" GDEXTENSION_ENTRY_POINT ${GDEXTENSION_ENTRY_POINT})
if (EXPORT_GODOT_PROJECT)
    set(DLLPATH "")
else()
    set(DLLPATH "${CMAKE_BINARY_DIR}/bin/")
endif()
configure_file(gdextension.in ${GDEXTENSION_CONFIGURATION_FILE})

# Build file locations
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/bin>
    LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/bin>
    RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>
)

# Installation of .gdextension and .dll file into project
# .dll is installed but not used during development.
# Manually reinstall when exporting the project
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${GDEXTENSION_CONFIGURATION_FILE}"
    CONFIGURATIONS Release Debug
    DESTINATION ${CMAKE_HOME_DIRECTORY}/project/bin
)
if (EXPORT_GODOT_PROJECT)
    install(
        FILES "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.dll"
        CONFIGURATIONS Release Debug
        DESTINATION ${CMAKE_HOME_DIRECTORY}/project/bin
    )
endif()