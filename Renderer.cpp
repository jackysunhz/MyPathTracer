//
// Created by goksu on 2/25/20.
//

#include <fstream>
#include "Renderer.h"


Renderer::Renderer(int _spp, bool use_mt, int maxt):
    spp(_spp), useMultiThreads(use_mt), maxThreads(maxt) {}

void Renderer::SetSpp(int _spp) { spp = _spp; }

void Renderer::EnableMultiThreading(bool usemt) { useMultiThreads = usemt; }

void Renderer::SetMaxThreads(int maxt) { maxThreads = maxt; }

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene)
{
    std::cout << "Number of samplings per pixel: " << spp << ".\n";
    int width = scene.GetWidth();
    int height = scene.GetHeight();
    std::vector<Vector3f> framebuffer(width * height);
    float scale = tan(deg2rad(scene.GetFov() * 0.5));
    float imageAspectRatio = width / (float)height;
    Vector3f eye_pos = scene.GetEyePos();

    if (useMultiThreads) {
        //All available threads are used by default, but you can also specify the maximum number of threads to use
        int max_threads = maxThreads == 0 ? omp_get_max_threads() : maxThreads;
        int height_per_thread = ceil(height / max_threads);
        int num_threads = ceil(height / height_per_thread);
        std::cout << "Number of threads used: " << num_threads << ".\n\n";
        std::cout << "Start rendering...\n\n";
        std::cout << "Rendering progress :\n";
        int progress = 0;
        #pragma omp parallel for
        for (int t = 0; t < num_threads; ++t) {
            int start_height = t * height_per_thread;
            int end_height = (t + 1) * height_per_thread;
            int m = start_height * width;
            //while(j < height_per_thread && j < height)
            for (uint32_t j = start_height; j < end_height && j < height; ++j) {
                for (uint32_t i = 0; i < width; ++i) {
                    for (int k = 0; k < spp; k++) {
                        // generate primary ray direction
                        float x = (2 * (i + get_random_float()) / (float)width - 1) *
                            imageAspectRatio * scale;
                        float y = (1 - 2 * (j + get_random_float()) / (float)height) * scale;
                        Vector3f dir = normalize(Vector3f(-x, y, 1));
                        Vector3f res = scene.Trace(Ray(eye_pos, dir), 0);
                        framebuffer[m] += res / spp;
                    }
                    m++;
                }
                #pragma omp critical
                progress++;
                UpdateProgress(progress / (float)height);
            }
            //printf("\nThread %d finished.\n", omp_get_thread_num());
        }
    }
    else {
        std::cout << "Running on single thread.\n\n";
        std::cout << "Start rendering...\n\n";
        std::cout << "Rendering progress :\n";
        int m = 0;
        for (uint32_t j = 0; j < height; ++j) {
            for (uint32_t i = 0; i < width; ++i) {
                float x = (2 * (i + 0.5) / (float)width - 1) * imageAspectRatio * scale;
                float y = (1 - 2 * (j + 0.5) / (float)height) * scale;
                Vector3f dir = normalize(Vector3f(-x, y, 1));
                for (int k = 0; k < spp; k++){
                    Vector3f res = scene.Trace(Ray(eye_pos, dir), 0);
                    framebuffer[m] += res / spp;  
                }
                m++;
            }
            UpdateProgress(j / (float)height);
        }
    }
    UpdateProgress(1.f);
    std::cout << "\n\nWriting to TheBox.ppm...\n";
    // save framebuffer to file
    FILE* fp = fopen("TheBox.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", width, height);
    for (auto i = 0; i < height * width; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
        color[1] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
        color[2] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
    std::cout << "Finished writing to TheBox.ppm.\n\n";
    std::cout << "Rendering complete. \n";
}


