#include "raylib.h"
#include "raymarcher3D.h"

bool started = false;

int main(){
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth,screenHeight,"raymarcher");

    RayMarcher3D marcher3d;
    Sphere3D s1({0,0,5},1.5f,RED);
    Sphere3D s2({2,0,7},1.0f,BLUE);
    Sphere3D s3({-2,0,6},1.0f,GREEN);

    Plane3D ground(-2.0f,LIGHTGRAY);

    marcher3d.addObject(&s1);
    marcher3d.addObject(&s2);
    marcher3d.addObject(&s3);

    marcher3d.addObject(&ground);

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if(!started){
            DrawText("3D RayMarcher",300,250,20,BLACK);

            Rectangle btn = {300,300,200,50};

            DrawRectangleRec(btn,LIGHTGRAY);
            DrawText("Start",360,315,20,BLACK);

            Vector2 mousePoint = GetMousePosition();
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(CheckCollisionPointRec(mousePoint,btn)){
                    started = true;
                }
            }
        }
        else{
           static Vector3 cameraPos = {0,0,-5}; 
           static float yaw = 0.0f; //left-right rotation
           static float pitch = 0.0f; //up-down rotation

           float moveSpeed = 0.1f;
           float rotSpeed = 0.02f;
        
           /// Forward direction (based on yaw only)
           Vector3 forward = {
            sinf(yaw),
            0,
            -cosf(yaw)
           };

           // Right direction
           Vector3 right = {
            cosf(yaw),
            0,
            sinf(yaw)
           };

           if(IsKeyDown(KEY_W)) {
            cameraPos.x += forward.x * moveSpeed;
            cameraPos.z += forward.z * moveSpeed;
           }
           if(IsKeyDown(KEY_S)) {
            cameraPos.x -= forward.x * moveSpeed;
            cameraPos.z -= forward.z * moveSpeed;
           }
           if(IsKeyDown(KEY_A)) {
            cameraPos.x -= right.x * moveSpeed;
            cameraPos.z -= right.z * moveSpeed;
           }
           if(IsKeyDown(KEY_D)) {
            cameraPos.x += right.x * moveSpeed;
            cameraPos.z += right.z * moveSpeed;
           }

           // vertical
           if(IsKeyDown(KEY_Q)) cameraPos.y -= moveSpeed;
           if(IsKeyDown(KEY_E)) cameraPos.y += moveSpeed;

           //rotation
           if(IsKeyDown(KEY_H)){
            yaw -= rotSpeed; //left
           }
           if(IsKeyDown(KEY_K)){
            yaw += rotSpeed; //right
           }
           if(IsKeyDown(KEY_U)){
            pitch += rotSpeed; //up
           }
           if(IsKeyDown(KEY_J)){
            pitch -= rotSpeed; //down
           }

           if(IsKeyDown(KEY_R)){
            cameraPos = {0,0,-5};
            yaw = 0;
            pitch = 0;
           }

           if(pitch > 1.5f){
            pitch = 1.5f;
           }
           if(pitch < -1.5f){
            pitch = -1.5f;
           }

           float fov = 60.0f;
           float aspect = screenWidth/(float)screenHeight;
           float scale = tanf((fov*0.5f)*(PI/180.0f));

           for(int y = 0;y<screenHeight;y+=2){
            for(int x = 0;x<screenWidth;x+=2){
                
                float ndcX = (2.0f*(x+0.5f)/screenWidth - 1.0f)*aspect*scale;
                float ndcY = (1.0f-2.0f*(y+0.5f)/screenHeight)*scale;

                Vector3 dir = {ndcX,ndcY,1.0f};
                
                //applying rotation (yaw+pitch)
                //yaw (around y axis)

                float cosY = cosf(yaw);
                float sinY = sinf(yaw);
                
                float dx = dir.x*cosY - dir.z*sinY;
                float dz = dir.x*sinY + dir.z*cosY;

                dir.x = dx;
                dir.z = dz;

                //pitch(around x axis)
                
                float cosP = cosf(pitch);
                float sinP = sinf(pitch);

                float dy = dir.y*cosP - dir.z*sinP;
                dz = dir.y*sinP + dir.z*cosP;

                dir.y = dy;
                dir.z = dz;

                //normalizing 
                float len = sqrtf(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
                dir.x = dir.x/len;
                dir.y = dir.y/len;
                dir.z = dir.z/len;

                Color col = marcher3d.marchRay(cameraPos,dir);
                DrawPixel(x,y,col);
            }

           }

        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}