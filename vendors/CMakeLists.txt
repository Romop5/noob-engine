cmake_minimum_required(VERSION 3.0)


set(IMGUI_SOURCES 
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
)

add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC imgui/.)

add_subdirectory(procgen)


add_library(bmploader INTERFACE)
target_include_directories(bmploader INTERFACE bmploader)

add_library(dirent INTERFACE)
target_include_directories(dirent INTERFACE dirent/.)
