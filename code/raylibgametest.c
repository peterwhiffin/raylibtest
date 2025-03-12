#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "string.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h";

typedef enum PlayerState{
    IDLE,
    WALK,
    SPRINT,
    JUMP
} PlayerState;

// typedef struct Transform {
//     Vector3 position;
//     Vector3 rotation;
//     Vector3 scalel;
//     Matrix transform;
// } Transform;

typedef struct Player {
    Transform transform;
    Vector3 velocity;
    PlayerState state;
} Player;

int main(void){

    const int screenWidth = 800;
    const int screenHeight = 600;
    //char resourcePath[128] = "x:/repos/raylibtest/resources/";
    const Color clearColor = {98, 110, 175, 255};
    
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Pete's House");

    Camera camera = { 0 };
    camera.fovy = 67;
    camera.position = (Vector3){0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.target = (Vector3){0.0f, 0.0f, 2.0f};

    Shader shader = LoadShader("x:/repos/raylibtest/resources/lighting.vs", "x:/repos/raylibtest/resources/lighting.fs");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    Light lights[MAX_LIGHTS] = { 0 };
    //lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -1, 1, -1}, Vector3Zero(), YELLOW, shader);
    //lights[1] = CreateLight(LIGHT_POINT, (Vector3){ 1, 1, 1}, Vector3Zero(), RED, shader);
    //lights[2] = CreateLight(LIGHT_POINT, (Vector3){ -1, 1, 1}, Vector3Zero(), PURPLE, shader);
    //lights[3] = CreateLight(LIGHT_POINT, (Vector3){ 1, 1, -1}, Vector3Zero(), WHITE, shader);
    Light sunlight = CreateLight(LIGHT_DIRECTIONAL, (Vector3){30.0f, 200.0f, 5.0f}, (Vector3){0.0f, 0.0f, 0.0f}, WHITE, shader);


    Model house = LoadModel("x:/repos/raylibtest/resources/TestWorld.m3d");
    Model mountains = LoadModel("x:/repos/raylibtest/resources/MountainSkybox.obj");
    Texture2D mountainTex = LoadTexture("x:/repos/raylibtest/resources/Skybox_Diff.png");
    SetMaterialTexture(&mountains.materials[0], MATERIAL_MAP_DIFFUSE, mountainTex);
    Vector3 housePosition = (Vector3) {0.0f, 0.0f, 0.0f};
    Vector3 mountainPosition = (Vector3) {0.0f, 150.0f, 0.0f};

    //house.materials[0].shader = shader;
    mountains.materials[0].shader = shader;
    for(int i = 0; i < house.materialCount; i++){
        house.materials[i].shader = shader;
    }

    DisableCursor();
    SetTargetFPS(144);

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FREE);


        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // lights[0].position = Vector3Lerp(lights[0].position, camera.position, .001f);

        // for(int i = 0; i < MAX_LIGHTS; i++){
        //     UpdateLightValues(shader, lights[i]);
        // }
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(camera);
    BeginShaderMode(shader);
    //DrawPlane(Vector3Zero(), (Vector2) { 10.0, 10.0 }, WHITE);
    //DrawCube(Vector3Zero(), 2.0, 4.0, 2.0, WHITE);
    DrawModel(house, housePosition, 25.0f, WHITE);
    DrawModel(mountains, mountainPosition, .01f, WHITE);
    EndShaderMode();
    EndMode3D();

    DrawFPS(10, 10);
    EndDrawing();
    }

    UnloadModel(mountains);
    UnloadModel(house);
    UnloadShader(shader);

    CloseWindow();
}

