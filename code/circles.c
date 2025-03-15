#include "stdio.h"
#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void)
{
    bool shouldExit = false;
    bool isCursorDisabled = false;
    bool drawRing = true;
    bool drawLines = true;
    bool editMode = false;

    int cameraProjection = CAMERA_ORTHOGRAPHIC;

    float guiOffset = .7;
    float currentAngle = PI;
    float speed = 2.0f;
    float radius = 2.0f;
    float trackerSize = 0.15f;
    float numberOfCircles = 3;
    float hue = 0.0f;

    Vector2 screenSize = {800, 600};
    Vector3 centerPosition = {0.0f, 0.0f, 0.0f};
    Vector3 trackerPosition = (Vector3){0.0f, 0.0f, 0.0f};
    Vector3 tempPosition = {0.0f, 0.0f, centerPosition.z};
    Vector3 color = {0.0f, 0.0f, 0.0f};

    Rectangle buttonRect = {0, 0, 30, 30};
    Rectangle baseSliderRect = {0, 0, 70, 30};
    Rectangle guiRect = {screenSize.x * guiOffset, 0, screenSize.x * (1 - guiOffset), screenSize.y};

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenSize.x, screenSize.y, "Pete's circles");
    SetExitKey(KEY_NULL);
    SetTargetFPS(144);

    Camera camera = {0};
    camera.fovy = 67;
    camera.position = (Vector3){-14.0f, 0.0f, -10.0f};
    camera.target = (Vector3){-14.0f, 0.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};

    while (!shouldExit)
    {
        if (WindowShouldClose())
        {
            shouldExit = true;
        }

        if (isCursorDisabled)
        {
            UpdateCamera(&camera, CAMERA_FREE);
        }

        if (IsWindowResized())
        {
            screenSize.x = GetScreenWidth();
            screenSize.y = GetScreenHeight();
            guiRect.x = screenSize.x * guiOffset;
            guiRect.width = screenSize.x * (1 - guiOffset);
            guiRect.height = screenSize.y;
        }

        if (GetKeyPressed() == KEY_ESCAPE)
        {
            isCursorDisabled = !isCursorDisabled;

            if (isCursorDisabled)
            {
                DisableCursor();
            }
            else
            {
                EnableCursor();
            }
        }

        currentAngle += speed * GetFrameTime();
        trackerPosition.x = centerPosition.x + radius * cos(currentAngle);
        trackerPosition.y = centerPosition.y + radius * sin(currentAngle);

        float angleStep = PI / max(1, floorf(numberOfCircles));
        float colorStep = 360 / numberOfCircles;
        BeginDrawing();
        ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawSphere(trackerPosition, trackerSize, BLUE);

                for (int i = 0; i < floorf(numberOfCircles); i++)
                {    
                    tempPosition.x = centerPosition.x + radius * cos(((i + 1) * angleStep) - currentAngle) * cos((i + 1) * angleStep);
                    tempPosition.y = centerPosition.y + radius * cos(((i + 1) * angleStep) - currentAngle) * sin((i + 1) * angleStep);
                    tempPosition.z = centerPosition.z;
                    DrawSphere(tempPosition, trackerSize, ColorFromHSV(colorStep * (i + 1), 1.0f, 1.0f));

                    if(drawLines){
                        tempPosition.x = centerPosition.x + radius * cos(((i + 1) * angleStep) - 0) * cos((i + 1) * angleStep);
                        tempPosition.y = centerPosition.y + radius * cos(((i + 1) * angleStep) - 0) * sin((i + 1) * angleStep);
                        Vector3 temp2 = tempPosition;
                        tempPosition.x = centerPosition.x + radius * cos(((i + 1) * angleStep) - (PI * 2) ) * cos((i + 1) * angleStep);
                        tempPosition.y = centerPosition.y + radius * cos(((i + 1) * angleStep) - (PI * 2) ) * sin((i + 1) * angleStep);
                        DrawLine3D(temp2, tempPosition, ColorFromHSV(colorStep * (i + 1), 1.0f, 1.0f));
                    }
                }

                if(drawRing){
                    DrawCircle3D(centerPosition, radius, (Vector3){0.0f, 1.0f, 0.0f}, 0.0f, BLUE);
                }

            EndMode3D();
                
            buttonRect.width = 30;
            buttonRect.x = screenSize.x - 10 - buttonRect.width;
            buttonRect.y = screenSize.y - 10 - buttonRect.height;
            baseSliderRect.width = guiRect.width * .5f;
            baseSliderRect.height = guiRect.height * .05;
            baseSliderRect.x = guiRect.x + (guiRect.width / 2) - (baseSliderRect.width / 2);
            baseSliderRect.y = guiRect.y + 50;

            DrawLine(guiRect.x, guiRect.y, guiRect.x, guiRect.height, Fade(LIGHTGRAY, 0.6f));
            DrawRectangle(guiRect.x, guiRect.y, guiRect.width, guiRect.height, Fade(LIGHTGRAY, 0.3f));
            
            baseSliderRect.y += baseSliderRect.height + 10;
            GuiSliderBar(baseSliderRect, "Radius", TextFormat("%6.2f", radius), &radius, 0.0, 10.0f);
            baseSliderRect.y += baseSliderRect.height + 10;
            GuiSliderBar(baseSliderRect, "Circles", TextFormat("%6.0f", floorf(numberOfCircles)), &numberOfCircles, 0.0, 100.0f);
            baseSliderRect.y += baseSliderRect.height + 10;
            GuiSliderBar(baseSliderRect, "Z Speed", TextFormat("%6.0f", floorf(numberOfCircles)), &numberOfCircles, 0.0, 100.0f);

            baseSliderRect.y += baseSliderRect.height + 10;

            baseSliderRect.height = 20;
            baseSliderRect.width = 20;
            GuiCheckBox(baseSliderRect, "Draw Ring", &drawRing);
            baseSliderRect.y += baseSliderRect.height + 10;

            GuiCheckBox(baseSliderRect, "Draw Lines", &drawLines);
            if (GuiButton(buttonRect, "Quit"))
            {
                shouldExit = true;
            }

            baseSliderRect.width = guiRect.width * .5f;
            baseSliderRect.height = guiRect.height * .05;
            baseSliderRect.x = guiRect.x + (guiRect.width / 2) - (baseSliderRect.width / 2);
            baseSliderRect.y = guiRect.y + 50;


            if (GuiDropdownBox(baseSliderRect, "PERSPECTIVE;ORTHOGRAPHIC", &camera.projection, editMode)){
                editMode = !editMode;
            }
            buttonRect.width += 50;
            buttonRect.x -= buttonRect.width + 10;
            
            if(GuiButton(buttonRect, "Reset Position")){
                camera.position = (Vector3){-14.0f, 0.0f, -10.0f};
                camera.target = (Vector3){-14.0f, 0.0f, 0.0f};
                centerPosition = (Vector3){0.0f, 0.0f, 0.0f};              
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}