# Renderer architecture

## Goals (things I want to learn)
* simple real-time rendering with shadows
* scene graph
* loading general formats (e.g. .obj) 
* optimalizations (octree)
* modern C++ with SDL2, GLM, CMake
* SSAO / bloom / FOV blur
* support for camera flights (curves, etc)
* bump mapping
* particle effetcs / particle systems

# Classes
 
SceneNode
    - composite pattern
    - has childrens 
    - is traversable (render)

VisualObject
    - inheritance of SceneNode
    - has some levels of details

Camera

Mesh
    - is part of visual object

TransformationObject
    - translation, rotation, etc
