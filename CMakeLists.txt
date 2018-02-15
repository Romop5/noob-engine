cmake_minimum_required(VERSION 3.0)

project(NoobEngine)

set(ENGINE_RESOURCES
    ./src/engine.cpp
)

add_subdirectory(vendors)

add_library(engine ${ENGINE_RESOURCES})
target_link_libraries(engine json)
target_include_directories(engine PRIVATE src)
