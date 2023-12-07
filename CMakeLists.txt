cmake_minimum_required(VERSION 3.5.0)
project(element-editor VERSION 0.0.1)

#include(CTest)
#enable_testing()

macro(override_option option type value)
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
file(COPY ./resources DESTINATION ${CMAKE_BINARY_DIR})

# compile options
set_property(TARGET element-editor PROPERTY CXX_STANDARD 20)
target_compile_options(element-editor PRIVATE -Wall)
override_option(BUILD_SHARED_LIBS BOOL OFF)

# define options
target_compile_definitions(element-editor PUBLIC ELM_EDITOR_VERSION="${PROJECT_VERSION}")
#LIBS 

# qt
set_target_properties(element-editor PROPERTIES AUTOMOC ON AUTOUIC ON)
target_link_libraries(element-editor PRIVATE Qt6::Widgets Qt6::WidgetsPrivate)

# element core
add_subdirectory(lib/element EXCLUDE_FROM_ALL)
target_link_libraries(element-editor PRIVATE element)

# assimp
override_option(ASSIMP_NO_EXPORT BOOL ON)
override_option(ASSIMP_INSTALL_PDB BOOL OFF)
override_option(ASSIMP_INSTALL BOOL OFF)
override_option(ASSIMP_BUILD_TESTS BOOL OFF)
override_option(ASSIMP_WARNINGS_AS_ERRORS BOOL OFF)
add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)
target_link_libraries(element-editor PUBLIC assimp)

# SPIRV-Tools
override_option(SPIRV_SKIP_TESTS BOOL ON)
override_option(SPIRV_SKIP_EXECUTABLES BOOL ON)
set(SPIRV-Headers_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/SPIRV-Headers)
add_subdirectory(lib/SPIRV-Tools EXCLUDE_FROM_ALL)

# SPIRV (glslang)
override_option(BUILD_TESTING BOOL OFF)
override_option(ENABLE_GLSLANG_BINARIES BOOL OFF)
override_option(ENABLE_SPVREMAPPER BOOL OFF)
override_option(ENABLE_HLSL BOOL OFF)
OVERRIDE_OPTION(SKIP_GLSLANG_INSTALL BOOL ON)
add_subdirectory(lib/glslang EXCLUDE_FROM_ALL)
add_dependencies(element-editor SPIRV)
target_link_libraries(element-editor PUBLIC SPIRV)

# SPIRV-Cross
override_option(SPIRV_CROSS_SHARED BOOL OFF)
override_option(SPIRV_CROSS_STATIC BOOL ON)
override_option(SPIRV_CROSS_CLI BOOL OFF)
override_option(SPIRV_CROSS_ENABLE_TESTS BOOL OFF)
override_option(SPIRV_CROSS_ENABLE_GLSL BOOL ON)
override_option(SPIRV_CROSS_ENABLE_HLSL BOOL OFF)
override_option(SPIRV_CROSS_ENABLE_MSL BOOL OFF)
override_option(SPIRV_CROSS_ENABLE_CPP BOOL OFF)
override_option(SPIRV_CROSS_ENABLE_REFLECT BOOL OFF)
override_option(SPIRV_CROSS_ENABLE_C_API BOOL OFF)
override_option(SPIRV_CROSS_ENABLE_UTIL BOOL OFF)
override_option(SPIRV_CROSS_SKIP_INSTALL BOOL OFF)
add_subdirectory(lib/SPIRV-Cross EXCLUDE_FROM_ALL)
target_link_libraries(element-editor PUBLIC spirv-cross-glsl)
target_include_directories(element-editor PUBLIC lib/SPIRV-Cross)

#end libs

# CMake options
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
