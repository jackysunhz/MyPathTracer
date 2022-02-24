#include "Renderer.h"
#include "Scene.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Vector.h"
#include "global.h"
#include "Material.h"
#include <chrono>

int main()
{
    Scene scene;
    Renderer r;

    Material* red = new Material(DIFFUSE, Vector3f(0.0f), Vector3f(0.63f, 0.065f, 0.05f));
    Material* green = new Material(DIFFUSE, Vector3f(0.0f), Vector3f(0.14f, 0.45f, 0.091f));
    Material* white = new Material(DIFFUSE, Vector3f(0.0f), Vector3f(0.725f, 0.71f, 0.68f));
    Material* light = new Material(DIFFUSE, Vector3f(47.83f, 38.57f, 31.08f), Vector3f(0.65f));
    Material* mf_al = new Material(MICROFACET, Vector3f(0.0f), Vector3f(0.92, 0.92, 0.92), 1.4f, 0.01f, 1);
    Material* mf_fe = new Material(MICROFACET, Vector3f(0.0f), Vector3f(0.12, 0.09, 0.07), 1.006f, 0.46f, 0.5);
    Material* mf_cu = new Material(MICROFACET, Vector3f(0.0f), Vector3f(0.95, 0.54, 0.44), 2.f, 0.15f, 1);

    Mesh* floor = new Mesh("../../../models/cornellbox/floor.obj", white);
    Mesh* shortbox = new Mesh("../../../models/cornellbox/shortbox.obj", white);
    Mesh* tallbox = new Mesh("../../../models/cornellbox/tallbox.obj", mf_al);
    Mesh* left = new Mesh("../../../models/cornellbox/left.obj", red);
    Mesh* right = new Mesh("../../../models/cornellbox/right.obj", green);
    Mesh* bunny = new Mesh("../../../models/bunny/bunny.obj", mf_cu);
    Mesh* teapot = new Mesh("../../../models/teapot/teapot.obj", mf_fe);
    Mesh* light_ = new Mesh("../../../models/cornellbox/light.obj", light);

    std::vector<Mesh*> objList{ floor, shortbox, tallbox, left, right, bunny, teapot, light_ };
    for (Mesh* obj : objList) {
        if (!obj->IsEmpty()) {
            scene.Add(obj);
        }
    }

    std::cout << "Start generating BVH...\n";
    auto start = std::chrono::system_clock::now();
    scene.BuildBVH();
    auto stop = std::chrono::system_clock::now();
    std::cout << "BVH generation completed in: " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n\n";

    /*Scene and Renderer Settings*/
    scene.SetFov(40);
    scene.SetResolution(784, 784);
    scene.SetRR(0.80);//Each ray is expected to bounce 1 / (1 - RR) times unless it hits light source directly
    r.EnableMultiThreading(true);//Set to false if openmp is not supported or you do not want to use multi threading
    r.SetMaxThreads(0);//You can specify max number of threads to use here(0 uses all available threads)
    r.SetSpp(64);//Set number of samplings per pixel(larger SSP usually generates less noisy result)

    start = std::chrono::system_clock::now();
    r.Render(scene);
    stop = std::chrono::system_clock::now();

    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";

    return 0;
}