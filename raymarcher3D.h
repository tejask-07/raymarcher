#pragma once
#include "raylib.h"
#include <vector>

struct SDF3D{
    virtual float distance(Vector3 p) = 0;
};

struct Sphere3D : SDF3D{
    Vector3 center;
    float radius;

    Sphere3D(Vector3 c , float r) : center(c) , radius(r) {}

    float distance(Vector3 p) override{
        float dx = p.x - center.x;
        float dy = p.y - center.y;
        float dz = p.z - center.z;
        return sqrtf(dx*dx + dy*dy + dz*dz) - radius;
    }
};

struct Plane3D : SDF3D{
    float y;

    Plane3D(float height) : y(height) {}

    float distance(Vector3 p) override{
        return p.y - y;
    }
};

struct RayMarcher3D{
    std::vector<SDF3D*> objects;

    int maxSteps = 100;
    float epsilon = 0.01f;
    float maxDistance = 100.0f;

    void addObject(SDF3D* obj);
    Color marchRay(Vector3 origin,Vector3 dir);
};