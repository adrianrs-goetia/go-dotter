project(gameplay-plugin CXX)

# Source files
set(SOURCES
    src/camerapivot.cpp
    src/core.cpp
    src/grapplecomponent.cpp
    src/inputcomponent.cpp
    src/mainnode.cpp
    src/parrycomponent.cpp
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
    PRIVATE godot::cpp
    PRIVATE debugdraw3d_api
    PRIVATE debug-draw-3d-library
)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_definitions(${PROJECT_NAME} PRIVATE HOT_RELOAD_ENABLED)

# Optional links
if(ENABLE_DD3D)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_DD3D)
endif()

# GDExtension file generation from gdextension.in
string(TOLOWER "${PROJECT_NAME}.gdextension" GDEXTENSION_CONFIGURATION_FILE)
string(TOLOWER ${PROJECT_NAME} GDEXTENSION_ENTRY_POINT)
string(REGEX REPLACE "-" "_" GDEXTENSION_ENTRY_POINT ${GDEXTENSION_ENTRY_POINT})
set(LIB_NAME_WINDOWS "${PROJECT_NAME}")
set(LIB_NAME_LINUX "lib${PROJECT_NAME}")

if(EXPORT_GODOT_PROJECT)
    if(MSVC)
        set(DLLPATH "")
    else() # not windows, assuming linux
        set(LINUXPATH "")
    endif()
else()
    if(MSVC)
        set(DLLPATH "${CMAKE_BINARY_DIR}/bin/")
    else() # not windows, assuming linux
        set(LINUXPATH "${CMAKE_CURRENT_BINARY_DIR}/bin/")
    endif()
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

if(EXPORT_GODOT_PROJECT)
    if (MSVC)
        set(LIBINSTALL "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.dll")
    else()
        set(LIBINSTALL "${CMAKE_CURRENT_BINARY_DIR}/bin/lib${PROJECT_NAME}.so")
    endif()
    install(
        FILES "${LIBINSTALL}"
        CONFIGURATIONS Release Debug
        DESTINATION ${CMAKE_HOME_DIRECTORY}/project/bin
    )
endif()