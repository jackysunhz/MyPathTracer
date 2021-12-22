//
// Created by goksu on 2/25/20.
//
#include "Scene.h"
#include <omp.h>

#pragma once

class Renderer
{
private:
    int spp;
    bool useMultiThreads;
    int maxThreads;
public:
    Renderer(int _spp = 1, bool use_mt = true, int maxt = 0);
    void SetSpp(int _spp);
    void EnableMultiThreading(bool usemt);
    void SetMaxThreads(int maxt);
    void Render(const Scene& scene);
};
