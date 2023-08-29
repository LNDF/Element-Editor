cmake_minimum_required(VERSION 3.5.0)
project(element-editor VERSION 0.0.1)

#include(CTest)
#enable_testing()

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

#end libs

# CMake options
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
