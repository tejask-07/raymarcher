#include "raylib.h"
#include "raymarcher2D.h"

int main(){
    InitWindow(800,600,"2Draymarch");

    RayMarcher2D marcher;
    Circle c1({400,300},100);
    marcher.addObject(&c1);

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //loopin thru pixels
        for(int y = 0 ; y < 600 ; y+=2){
            for(int x = 0 ; x < 800 ; x+=2){
                Vector2 origin = {100,300}; //camera origin
                Vector2 dir = {x - origin.x,y-origin.y};
                float len = sqrt(dir.x*dir.x + dir.y*dir.y);
                dir.x /= len;
                dir.y /= len;

                Color col = marcher.marchRay(origin,dir);
                DrawPixel(x,y,col);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}