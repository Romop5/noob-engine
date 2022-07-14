cmake_minimum_required(VERSION 3.0)

project(NoobEngine)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

########
# Engine
########
set(ENGINE_RESOURCES
    ./src/engine/engine.cpp
    ./src/gui/gui.cpp
    ./src/gui/imgui_impl_sdl_gl3.cpp
)

add_subdirectory(vendors)

find_package(Threads REQUIRED)

add_library(engine ${ENGINE_RESOURCES})
target_link_libraries(engine json  CONAN_PKG::sdl CONAN_PKG::opengl CONAN_PKG::glew CONAN_PKG::libffi imgui bmploader dirent) 
target_include_directories(engine PUBLIC src)

########
# Viewer 
########

set(VIEWER_RESOURCES
    ./src/viewer/main.cpp
)
add_executable(viewer ${VIEWER_RESOURCES})
target_link_libraries(viewer engine procgen)


add_executable(editor ./src/viewer/editor.cpp) 
target_link_libraries(editor engine procgen ${CMAKE_THREAD_LIBS_INIT})


add_executable(viewer-texture ./src/viewer/textureviewer.cpp) 
target_link_libraries(viewer-texture engine procgen)


get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

