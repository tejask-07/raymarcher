#pragma once
#include "raylib.h"
#include "sdf2D.h"
#include <vector>

struct RayMarcher2D {
    std::vector<SDF2D*> objects;
    int maxSteps = 100;
    float epsilon = 0.5f;
    float maxDistance = 500.0f;

    void addObject(SDF2D* obj);
    Color marchRay(Vector2 origin,Vector2 direction);
};