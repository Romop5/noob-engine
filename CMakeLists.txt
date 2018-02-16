cmake_minimum_required(VERSION 3.0)

project(NoobEngine)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

########
# Engine
########
set(ENGINE_RESOURCES
    ./src/engine/engine.cpp
)

add_subdirectory(vendors)

find_package(GLM REQUIRED)
add_library(engine ${ENGINE_RESOURCES})
target_link_libraries(engine json)
target_include_directories(engine PUBLIC src ${GLM_INCLUDE_DIRS})

########
# Viewer 
########

set(VIEWER_RESOURCES
    ./src/viewer/main.cpp
)
add_executable(viewer ${VIEWER_RESOURCES})
target_link_libraries(viewer engine)


get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

