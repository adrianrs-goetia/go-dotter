project(fsm-plugin CXX)

# Shared library
set(SOURCES
    src/state.cpp
    src/playerstate.cpp
)
set(HEADER
    src/state.h
    src/playerstate.h
)
add_library(
    ${PROJECT_NAME}
    SHARED
    register_types.cpp # Always added
    ${HEADERS}
    ${SOURCES}
)
target_link_libraries(${PROJECT_NAME} PUBLIC godot-cpp)
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
configure_file(gdextension.in ${GDEXTENSION_CONFIGURATION_FILE})

# Build file locations
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/bin>
    LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/bin>
    RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/bin>
)

# Installation of .gdextension and .dll file into project
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