#pragma once
#include "raylib.h"
#include "raymath.h"

struct SDF2D{
    virtual float distance(Vector2 p) = 0;
};

struct Circle : SDF2D{
    Vector2 center;
    float radius;

    Circle(Vector2 c,float r) : center(c) , radius(r) {}
    float distance(Vector2 p) override {
        return Vector2Distance(p,center) - radius;
    }
};

struct Box : SDF2D{
    Vector2 center;
    Vector2 halfSize;

    Box(Vector2 c,Vector2 hs) : center(c) , halfSize(hs) {}
    float distance(Vector2 p) override {
        Vector2 d = {fabsf(p.x - center.x) - halfSize.x,fabsf(p.y-center.y)-halfSize.y};
        return fmaxf(d.x,0.0f) + fmaxf(d.y,0.0f) + fminf(fmaxf(d.x,d.y),0.0f);
    }
};