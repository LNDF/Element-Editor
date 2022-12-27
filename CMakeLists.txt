cmake_minimum_required(VERSION 3.0.0)
project(element-editor VERSION 0.0.1)

#include(CTest)
#enable_testing()

# source files
file(GLOB_RECURSE SRC_FILES ./src/*.*)
add_executable(element-editor ${SRC_FILES})
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


# element core
add_subdirectory(lib/element EXCLUDE_FROM_ALL)
target_link_libraries(element-editor PRIVATE element)

#end libs

# CMake options
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
