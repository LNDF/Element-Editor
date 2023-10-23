cmake_minimum_required(VERSION 3.5.0)
project(element-editor VERSION 0.0.1)

#include(CTest)
#enable_testing()

macro(OVERRIDE_OPTION option type value)
  set(${option} ${value} CACHE ${type} "" FORCE)
endmacro()

#Qt
find_package(Qt6 COMPONENTS Widgets REQUIRED)
qt_standard_project_setup()
# only use moc and uic on current target
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)

# source files
file(GLOB_RECURSE SRC_FILES ./src/*.*)
file(GLOB_RECURSE UI_FILES ,/src/*.ui)
add_executable(element-editor ${SRC_FILES} ${UI_FILES})
target_include_directories(element-editor PUBLIC ./src/)

# asset files
file(GLOB_RECURSE RESOURCE_FILES ./resources/*.*)
file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_BINARY_DIR})

# compile options
set_property(TARGET element-editor PROPERTY CXX_STANDARD 20)
target_compile_options(element-editor PRIVATE -Wall)

# define options
target_compile_definitions(element-editor PUBLIC ELM_EDITOR_VERSION="${PROJECT_VERSION}")
#LIBS 

# qt
set_target_properties(element-editor PROPERTIES AUTOMOC ON AUTOUIC ON)
target_link_libraries(element-editor PRIVATE Qt6::Widgets Qt6::WidgetsPrivate)

# element core
add_subdirectory(lib/element EXCLUDE_FROM_ALL)
target_link_libraries(element-editor PRIVATE element)

# Vulkan (shaderc)
find_package(Vulkan COMPONENTS shaderc_combined REQUIRED)
target_link_libraries(element-editor PUBLIC Vulkan::shaderc_combined)

# SPIRV-Cross
OVERRIDE_OPTION(SPIRV_CROSS_SHARED BOOL OFF)
OVERRIDE_OPTION(SPIRV_CROSS_STATIC BOOL ON)
OVERRIDE_OPTION(SPIRV_CROSS_CLI BOOL OFF)
OVERRIDE_OPTION(SPIRV_CROSS_ENABLE_TESTS BOOL OFF)
OVERRIDE_OPTION(SPIRV_CROSS_ENABLE_GLSL BOOL ON)
OVERRIDE_OPTION(SPIRV_CROSS_ENABLE_HLSL BOOL OFF)
OVERRIDE_OPTION(SPIRV_CROSS_ENABLE_MSL BOOL OFF)
OVERRIDE_OPTION(SPIRV_CROSS_ENABLE_CPP BOOL OFF)
OVERRIDE_OPTION(SPIRV_CROSS_ENABLE_REFLECT BOOL OFF)
OVERRIDE_OPTION(SPIRV_CROSS_ENABLE_C_API BOOL OFF)
OVERRIDE_OPTION(SPIRV_CROSS_ENABLE_UTIL BOOL OFF)
OVERRIDE_OPTION(SPIRV_CROSS_SKIP_INSTALL BOOL OFF)
add_subdirectory(lib/SPIRV-Cross EXCLUDE_FROM_ALL)
target_link_libraries(element-editor PUBLIC spirv-cross-glsl)
target_include_directories(element-editor PUBLIC lib/SPIRV-Cross)

#end libs

# CMake options
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
