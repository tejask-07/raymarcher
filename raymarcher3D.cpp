#include "raymarcher3D.h"
#include<cmath>

void RayMarcher3D :: addObject(SDF3D*obj){
    objects.push_back(obj);
}

Color RayMarcher3D :: marchRay(Vector3 origin,Vector3 dir){
    float totalDist = 0.0f;
    Vector3 p = origin;

    for(int i = 0;i < maxSteps;i++){
        float minDist = maxDistance;

        for(auto obj : objects){
            float d = obj->distance(p);
            if(d < minDist){
                minDist = d;
            }
        }

        if(minDist < epsilon){
            //simple shade based on steps
            int shade = 255 - i*2;
            if(shade < 0){
                shade = 0;
            }
            return (Color){
                static_cast<unsigned char>(shade),
                static_cast<unsigned char>(shade),
                static_cast<unsigned char>(shade),
                255};
        }

        totalDist += minDist;

        p.x = origin.x + dir.x*totalDist;
        p.y = origin.y + dir.y*totalDist;
        p.z = origin.z + dir.z*totalDist;
        
        if(totalDist > maxDistance){
            break;
        }
    }
    return BLACK;
}