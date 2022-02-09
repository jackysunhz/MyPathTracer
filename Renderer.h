#pragma once
#include "Scene.h"
#include <omp.h>

class Renderer {
private:
    int spp;//samples per pixel
    bool useMultiThreads;
    int maxThreads;//maximum # of threads to use
public:
    //constructor
    Renderer(int _spp = 1, bool use_mt = true, int maxt = 0);

    void SetSpp(int _spp);
    void EnableMultiThreading(bool usemt);
    void SetMaxThreads(int maxt);

    //render the scene
    void Render(const Scene& scene);
};
