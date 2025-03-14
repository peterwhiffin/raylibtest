#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "stdio.h"
#define MAX_BUILDINGS 100

void WindowResized(Vector2 *center);
const int screenWidth = 800;
const int screenHeight = 600;

Vector2 screenSize = { 800, 600 };

int main(void) {
    
    SetConfigFlags(FLAG_MSAA_4X_HINT|FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Pete's circles");

    SetExitKey(KEY_NULL); 
    bool shouldExit = false;
    bool isCursorDisabled = true;

    Vector2 screenCenter = { (screenWidth - 300) / 2.0f, screenHeight / 2.0f };

    Camera camera = { 0 };
    camera.fovy = 67;
    camera.position = (Vector3) {0.0f, 0.0f, 0.0f};
    camera.target = (Vector3) {0.0f, 0.0f, 2.0f};
    camera.projection = CAMERA_PERSPECTIVE;
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    

    Vector3 cubePosition = {0.0f, 0.0f, 5.0f};
    float guiOffset = .7;

    float currentAngle = 0.0f;
    float speed = 2.0f;
    float radius = 1.0f;
    float trackerSize = 0.2f;
    float numberOfCircles = 6;

    Rectangle buttonRect = {0, 0, 30, 30};

    Vector3 centerPosition = {0.0f, 0.0f, 0.0f};

    Rectangle baseSliderRect = {0, 0, 70, 30};

    Vector3 trackerPosition = (Vector3){0.0f, 0.0f, 0.0f};
    Vector3 trackerRotationAxis = (Vector3){0, 1, 0};

    SetTargetFPS(144);  
    DisableCursor();
    

    while (!shouldExit) {

        if(WindowShouldClose()){
            shouldExit = true;
        }

        if(!isCursorDisabled){
            UpdateCamera(&camera, CAMERA_FREE);
        }
       

        if(IsWindowResized()) {        
            WindowResized(&screenCenter);
        }

        if(GetKeyPressed() == KEY_ESCAPE){
            isCursorDisabled = !isCursorDisabled;

            if(isCursorDisabled){
                EnableCursor();
            }
            else{
                DisableCursor();
            }
        }

        currentAngle += speed * GetFrameTime();
        trackerPosition.x = centerPosition.x + radius * cos(currentAngle);
        trackerPosition.y = centerPosition.y + radius * sin(currentAngle);

        float angleStep = PI / max(1, numberOfCircles);  

        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            BeginMode3D(camera);
                DrawSphere(trackerPosition, trackerSize, BLUE);

                for(int i = 0; i < floorf(numberOfCircles); i++){
                    Vector3 position = {0.0f, 0.0f, centerPosition.z};
                    position.x = centerPosition.x + radius * cos(((i + 1) * angleStep) - currentAngle) * cos((i + 1) * angleStep);
                    position.y = centerPosition.y + radius * cos(((i + 1) * angleStep) - currentAngle) * sin((i + 1) * angleStep);
                    DrawSphere(position, trackerSize, RED);
                }

            EndMode3D();

            Rectangle guiRect = {screenSize.x * guiOffset, 0, screenSize.x * (1 - guiOffset), screenSize.y};

            buttonRect.x = screenSize.x - 10 - buttonRect.width;
            buttonRect.y = screenSize.y - 10 - buttonRect.height;

            if(GuiButton(buttonRect, "Quit")){
                shouldExit = true;
            }

            baseSliderRect.width = guiRect.width * .5f;
            baseSliderRect.height = guiRect.height * .05;
            baseSliderRect.x = guiRect.x + (guiRect.width / 2) - (baseSliderRect.width / 2);
            baseSliderRect.y = guiRect.y + 50;
            
            GuiSliderBar(baseSliderRect, "Radius", TextFormat("%6.2f", radius), &radius, 0.0, 10.0f);

            baseSliderRect.y += baseSliderRect.height + 10;

            GuiSliderBar(baseSliderRect, "Circles", TextFormat("%6.0f", floorf(numberOfCircles)), &numberOfCircles, 0.0, 100.0f);
            DrawLine(guiRect.x, guiRect.y, guiRect.x, guiRect.height, Fade(LIGHTGRAY, 0.6f));
            DrawRectangle(guiRect.x, guiRect.y, guiRect.width, guiRect.height, Fade(LIGHTGRAY, 0.3f));
   
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void WindowResized(Vector2 *center) {
    screenSize.x = GetScreenWidth();
    screenSize.y = GetScreenHeight();
    *center = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
}