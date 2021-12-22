#include "Scene.h"
void Scene::SampleLight(Intersection &inter, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(inter, pdf);
                break;
            }
        }
    }
}

Vector3f Scene::Trace(const Ray& ray, int depth) const
{
    if (depth > this->maxDepth) {
        return Vector3f(0.0, 0.0, 0.0);
    }
    Intersection intersection = Scene::GetIntersection(ray);
    if (!intersection.happened) {
        return Vector3f(0.0, 0.0, 0.0);
    }
    if (intersection.m->hasEmission()) {
        return intersection.m->getEmission() / RussianRoulette * dotProduct(-ray.direction, intersection.normal);
    }
    Vector3f s = Shade(intersection, ray.direction);
    s.x = clamp(0, 1, s.x);
    s.y = clamp(0, 1, s.y);
    s.z = clamp(0, 1, s.z);
    return s;
}

Vector3f Scene::Shade(const Intersection& inter, const Vector3f& wi) const {
    //get the sampling position and pdf of the light
    Intersection lightSampleInter = Intersection();
    float lightPdf = 0;
    SampleLight(lightSampleInter, lightPdf);

    Vector3f l_dir = Vector3f(0, 0, 0);
    Vector3f x = lightSampleInter.coords;
    Vector3f nn = lightSampleInter.normal;
    Material* m = inter.m;
    Vector3f p = inter.coords;
    Vector3f n = inter.normal;
    Vector3f wo = (x - p).normalized();
    float d = (x - p).norm();
    Intersection lightSampleHit = Scene::GetIntersection(Ray(p, wo));
    if (lightSampleHit.distance + EPSILON >= d) {//direct lighting is not blocked
        float cos2 = dotProduct(-wo, nn);
        if (cos2 > 0) {
            l_dir = lightSampleInter.emit * m->brdf(wi, wo, n) * dotProduct(wo, n) * dotProduct(-wo, nn) / pow(d, 2) / lightPdf;
        }
    }
    Vector3f l_indir = Vector3f(0, 0, 0);
    if (get_random_float() < RussianRoulette) {
        wo = m->sample(wi, n);
        float pdf = m->pdf(wi, wo, n);
        Intersection diffuseSampleInter = Scene::GetIntersection(Ray(p, wo));
        if (diffuseSampleInter.happened && !diffuseSampleInter.m->hasEmission()) {
            l_indir = Shade(diffuseSampleInter, wo) * m->brdf(wi, wo, n) / pdf / RussianRoulette * dotProduct(wo, n);
        }
    }
    return l_dir + l_indir;
}
