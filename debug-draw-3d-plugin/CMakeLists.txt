if (NOT ENABLE_DD3D)
return()
endif()

project(debug-draw3d-plugin CXX)

# Source files
set(SOURCES
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
    ${PROJECT_NAME}-obj
    OBJECT
    ${SOURCES}
)
target_link_libraries(${PROJECT_NAME}-obj PRIVATE godot-cpp)
target_include_directories(
    ${PROJECT_NAME}-obj
    INTERFACE
    $<INSTALL_INTERFACE:src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
target_include_directories(${PROJECT_NAME}-obj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/debugdraw3d)
