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

   Vector3 lightPos = {5,5,-5}; 

   for(int i = 0;i<maxSteps;i++){
    float minDist = maxDistance;

    for(auto obj : objects){
        float d = obj->distance(p);
        if(d < minDist){
            minDist = d;
        }
    }
    
    //hit
    if(minDist < epsilon){
        
        Vector3 normal = getNormal(p,objects);

        Vector3 lightDir = {
            lightPos.x - p.x,
            lightPos.y - p.y,
            lightPos.z - p.z
        };

        float len = sqrtf(lightDir.x*lightDir.x + lightDir.y*lightDir.y + lightDir.z*lightDir.z);
        lightDir.x /= len;
        lightDir.y /= len;
        lightDir.z /= len;

        //diffuse
        float diff = normal.x*lightDir.x + normal.y*lightDir.y + normal.z*lightDir.z;
        diff = fmaxf(diff,0.0f);

        float shadow = 1.0f;
        float t = 0.02f;

        for(int j = 0 ; j < 50 ; j++){
            Vector3 sp = {
                p.x + lightDir.x*t,
                p.y + lightDir.y*t,
                p.z + lightDir.z*t
            };

            float h = maxDistance;
            for(auto obj : objects){
                float d = obj->distance(sp);
                if(d < h){
                    h = d;
                }
            }

            if(h < 0.001f){
                shadow = 0.2f; //in shadow (dtm) 
                break;
            }

            t+=h;
            if(t > 20.0f){
                break;
            }
        }

        float ambient = 0.2f;
        float lighting = ambient + diff*shadow*0.8f;

        float fog = expf(-0.02f*totalDist);
        lighting*=fog;

        int shade = (int)(lighting*255);
        shade = (shade < 0) ? 0 : (shade > 255 ? 255 : shade);

        return Color{
            (unsigned char)(shade),
            (unsigned char)(shade*0.9f),
            (unsigned char)(shade*0.8f),
            255
        };
    }

    totalDist += minDist;

    if(totalDist > maxDistance){
        break;
    }

    p.x = origin.x + dir.x*totalDist;
    p.y = origin.y + dir.y*totalDist;
    p.z = origin.z + dir.z*totalDist;
   }

   float t = 0.5f*(dir.y+1.0f);
   return Color{
    (unsigned char)((1.0f - t)*200 + t*135),
    (unsigned char)((1.0f - t)*220 + t*206),
    (unsigned char)((1.0f - t)*255 + t*235),
    255
   };
   
}