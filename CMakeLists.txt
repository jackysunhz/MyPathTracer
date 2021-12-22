cmake_minimum_required(VERSION 3.10)
project(MyPathTracer)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)

add_executable(MyPathTracer main.cpp "Object.h" "global.h" "Triangle.h" Scene.cpp
        "Scene.h"   BVH.cpp "BVH.h" "BoundingBox.h" "Ray.h" Material.cpp "Material.h" "Intersection.h"
        Renderer.cpp "Renderer.h" "Mesh.h" )

if(OpenMP_CXX_FOUND)
    target_link_libraries(MyPathTracer PUBLIC OpenMP::OpenMP_CXX)
endif()

include_directories("external/")