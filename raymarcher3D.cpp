#include "raymarcher3D.h"
#include<cmath>

void RayMarcher3D :: addObject(SDF3D*obj){
    objects.push_back(obj);
}

//calc the surface normal at the hitpoint using the sdf & diffuse light to shade the sphere proprly cause the color was only dependent on the stepcounts also no light calc so it was flat 
Vector3 getNormal(Vector3 p,std::vector<SDF3D*>& objects){
    float eps = 0.001f;

    auto sceneDist = [&](Vector3 pt){
        float minDist = 1e9;
        for(auto obj : objects){
            float d = obj->distance(pt);
            if(d < minDist){
                minDist = d;
            }
        }
        return minDist;
    };

    float dx = sceneDist({p.x + eps,p.y,p.z}) - sceneDist({p.x - eps,p.y,p.z});
    float dy = sceneDist({p.x,p.y + eps,p.z}) - sceneDist({p.x,p.y - eps,p.z});
    float dz = sceneDist({p.x,p.y,p.z + eps}) - sceneDist({p.x,p.y,p.z-eps});

    Vector3 n = {dx,dy,dz};
    float len = sqrtf(n.x*n.x + n.y*n.y + n.z*n.z);
    if(len == 0.0f){
        return {0,0,0};
    }
    
    n.x = n.x/len;
    n.y = n.y/len;
    n.z = n.z/len;

    return {n.x,n.y,n.z};
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
            //light-direction
            Vector3 lightDir = {-1,-1,-1};
            float len = sqrtf(lightDir.x*lightDir.x + lightDir.y*lightDir.y + lightDir.z*lightDir.z);
            lightDir.x = lightDir.x/len;
            lightDir.y = lightDir.y/len;
            lightDir.z = lightDir.z/len;

            //surface-normal
            Vector3 normal = getNormal(p,objects);

            //diffuse shading
            float diff = normal.x*lightDir.x + normal.y*lightDir.y + normal.z*lightDir.z;
            if(diff < 0){
                diff = 0;
            }

            float ambient = 0.2f;
            float lighting = ambient + diff*0.8f;

            //convertin color
            int shade = (int)(lighting*255);
            if(shade < 0){
                shade = 0; //ensures no weird ahh overflow
            }
            if(shade > 255){
                shade = 255; //samehere
            }
            return Color{
                (unsigned char)shade,
                (unsigned char)shade,
                (unsigned char)shade,
                255
            };
        }

        totalDist += minDist;

        p.x = origin.x + dir.x*totalDist;
        p.y = origin.y + dir.y*totalDist;
        p.z = origin.z + dir.z*totalDist;
        
        if(totalDist > maxDistance){
            break;
        }
    }
    return RED;
}