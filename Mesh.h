#pragma once
#include "BVH.h"
#include "Intersection.h"
#include "Material.h"
#include "OBJ_Loader.h"
#include "Object.h"
#include <cassert>
#include <array>

class Mesh: public Object {
private:
    BoundingBox bounding_box;//BB of the whole mesh
    BVH* bvh;//bvh that breaks down triangles in this mesh
    float area = 0;//total surface area
    Material* m;
    std::vector<Triangle> triangles;//triangles of the mesh
    uint32_t numTriangles = 0;
    std::unique_ptr<Vector3f[]> vertices;
    std::unique_ptr<uint32_t[]> vertexIndex;
public:
    //constructor
    Mesh(const std::string& filename, Material* mt = new Material()): m(mt) {
        //load file using the obj loader (https://github.com/Bly7/OBJ-Loader)
        objl::Loader loader;
        try {
            loader.LoadFile(filename);
            if (loader.LoadedMeshes.size() != 1) {
                throw std::invalid_argument("Location " + filename + " cannot be found.\n");
            }
        }
        catch(const std::invalid_argument& e){
            std::cout << filename + " is not loaded, please check the file path provided." << std::endl;
            return;
        }
        assert(loader.LoadedMeshes.size() == 1);//throw error if mesh is not loaded
        auto mesh = loader.LoadedMeshes[0];
        Vector3f min_vert = Vector3f{ std::numeric_limits<float>::infinity(),
                                     std::numeric_limits<float>::infinity(),
                                     std::numeric_limits<float>::infinity() };
        Vector3f max_vert = Vector3f{ -std::numeric_limits<float>::infinity(),
                                     -std::numeric_limits<float>::infinity(),
                                     -std::numeric_limits<float>::infinity() };
        for (int i = 0; i < mesh.Vertices.size(); i += 3) {
            std::array<Vector3f, 3> face_vertices;
            for (int j = 0; j < 3; j++) {
                auto vert = Vector3f(mesh.Vertices[i + j].Position.X,
                    mesh.Vertices[i + j].Position.Y,
                    mesh.Vertices[i + j].Position.Z);
                face_vertices[j] = vert;
                min_vert = Vector3f(std::min(min_vert.x, vert.x),
                    std::min(min_vert.y, vert.y),
                    std::min(min_vert.z, vert.z));
                max_vert = Vector3f(std::max(max_vert.x, vert.x),
                    std::max(max_vert.y, vert.y),
                    std::max(max_vert.z, vert.z));
            }
            triangles.emplace_back(face_vertices[0], face_vertices[1],
                face_vertices[2], mt);
        }
        bounding_box = BoundingBox(min_vert, max_vert);
        std::vector<Object*> ptrs;
        for (auto& tri : triangles) {
            ptrs.push_back(&tri);
            numTriangles += 1;
            area += tri.getArea();
        }
        bvh = new BVH(ptrs);
    }

    bool IsEmpty() { return triangles.empty(); }

    BoundingBox getBounds() { return bounding_box; }

    Intersection getIntersection(Ray ray) { return bvh ? bvh->GetIntersection(ray) : Intersection(); }

    float getArea() { return area; }

    bool hasEmit() { return m->HasEmission(); }

    void Sample(Intersection& inter, float& pdf) {
        bvh->GetSample(inter, pdf);
        inter.emit = m->GetEmission();
    }
};
