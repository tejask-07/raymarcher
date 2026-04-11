#pragma once
#include "raylib.h"
#include <vector>

struct SDF3D{
    Color color;
    virtual float distance(Vector3 p) = 0;
};

struct Sphere3D : SDF3D{
    Vector3 center;
    float radius;

    Sphere3D(Vector3 c , float r,Color col){
        center = c;
        radius = r;
        color = col;
    }

    float distance(Vector3 p) override{
        float dx = p.x - center.x;
        float dy = p.y - center.y;
        float dz = p.z - center.z;
        return sqrtf(dx*dx + dy*dy + dz*dz) - radius;
    }
};

struct Plane3D : SDF3D{
    float y;

    Plane3D(float height,Color col){
        y = height;
        color = col;
    }

    float distance(Vector3 p) override{
        return p.y - y;
    }
};

struct RayMarcher3D{
    std::vector<SDF3D*> objects;

    int maxSteps = 60;
    float epsilon = 0.02f;
    float maxDistance = 100.0f;

    void addObject(SDF3D* obj);
    Color marchRay(Vector3 origin,Vector3 dir,int depth = 0);
};