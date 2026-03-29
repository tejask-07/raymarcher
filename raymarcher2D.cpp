#include "raymarcher2D.h"
#include<cmath>

void RayMarcher2D :: addObject(SDF2D* obj){
    objects.push_back(obj);
}

Color RayMarcher2D :: marchRay(Vector2 origin , Vector2 direction){
    float totalDist = 0;
    Vector2 p = origin;

    for(int i = 0;i<maxSteps;i++){
        float minDist = maxDistance;

        //finding closest dist to any object
        for(auto obj : objects){
            float d = obj->distance(p);
            if(d < minDist){
                minDist = d;
            }
        }

        if(minDist < epsilon){
            return RED; //basically hit
        }

        totalDist += minDist;
        p.x = origin.x + direction.x*totalDist;
        p.y = origin.y + direction.y*totalDist;

        if(totalDist > maxDistance){
            break;
        }
    }
    return BLACK; //no hit
}
