
#include "raylib.h"
#include "rcamera.h"
#include "stdio.h"
#include "stdlib.h"
#include "rlgl.h"
#define GRAPHICS_API_OPENGL_33
#define CHUNK_SIZE 16
#define WORLD_SIZE 2
#define MAX_VERTICES CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 24
#define MAX_INDICES CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 12

typedef enum{
    RIGHT,
    LEFT,
    TOP,
    BOTTOM,
    FRONT,
    BACK
} Face;

typedef struct{
    Vector3 position;
    Color color;
    bool isActive;
} Voxel;

typedef struct{   
    Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];    
} Chunk;

typedef struct{
    Chunk* chunks;
} World;

typedef struct{
    float* vertices;
    float* texCoords;
    unsigned short* indices;
    int vertexCount;
    unsigned short indexCount;
} MeshData;

void createChunks(World* world, Color* palette);

void WeGottaGo(char message[], int exitCode){
    printf(message);
    exit(exitCode);
}



Mesh createRaylibMesh(MeshData* meshData) {
    Mesh mesh = { 0 };
    
    // Create vertex buffer
    mesh.vertexCount = meshData->vertexCount;
    mesh.vertices = (float*)malloc(sizeof(float) * mesh.vertexCount * 3);  // 3 for x, y, z
    memcpy(mesh.vertices, meshData->vertices, sizeof(float) * mesh.vertexCount * 3);

    // Create texture coordinates buffer
    mesh.texcoords = (float*)malloc(sizeof(float) * mesh.vertexCount * 2);  // 2 for u, v
    memcpy(mesh.texcoords, meshData->texCoords, sizeof(float) * mesh.vertexCount * 2);
    
    // Create indices buffer
    printf("before triangle count\n");
    mesh.triangleCount = meshData->indexCount / 3;  // Each triangle uses 3 indices
    printf("after triangle count\n");
    mesh.indices = (unsigned short*)malloc(sizeof(unsigned short) * meshData->indexCount);
    memcpy(mesh.indices, meshData->indices, sizeof(unsigned short) * meshData->indexCount);
    
    return mesh;
}

void addFaceData(Face face, MeshData* meshData, int x, int y, int z){
    switch(face){
        case RIGHT:{
        float faceVertices[4][3] = {
            {x + 1, y, z + 1}, {x + 1, y, z}, {x + 1, y + 1, z}, {x + 1, y + 1, z + 1}
        };
    
        for (int i = 0; i < 4; i++) {
            meshData->vertices[meshData->vertexCount * 3 + 0] = faceVertices[i][0];
            meshData->vertices[meshData->vertexCount * 3 + 1] = faceVertices[i][1];
            meshData->vertices[meshData->vertexCount * 3 + 2] = faceVertices[i][2];

            meshData->texCoords[meshData->vertexCount * 2 + 0] = (i == 1 || i == 2) ? 1.0f : 0.0f;
            meshData->texCoords[meshData->vertexCount * 2 + 1] = (i == 2 || i == 3) ? 1.0f : 0.0f;
    
            (meshData->vertexCount)++;
        }
    
        unsigned int faceIndices[6] = {0, 1, 2, 0, 2, 3};
        for (int i = 0; i < 6; i++) {
            meshData->indices[meshData->indexCount++] = (meshData->vertexCount - 4 + faceIndices[i]);
            (meshData->indexCount)++;
        }
    }
        break;
        case LEFT:
        {
        float faceVertices[4][3] = {
            {x, y, z}, {x, y, z + 1}, {x, y + 1, z + 1}, {x, y + 1, z}
        };
    
        for (int i = 0; i < 4; i++) {
            meshData->vertices[meshData->vertexCount * 3 + 0] = faceVertices[i][0];
            meshData->vertices[meshData->vertexCount * 3 + 1] = faceVertices[i][1];
            meshData->vertices[meshData->vertexCount * 3 + 2] = faceVertices[i][2];

            meshData->texCoords[meshData->vertexCount * 2 + 0] = (i == 1 || i == 2) ? 1.0f : 0.0f;
            meshData->texCoords[meshData->vertexCount * 2 + 1] = (i == 2 || i == 3) ? 1.0f : 0.0f;
    
            (meshData->vertexCount)++;
        }
    
        unsigned int faceIndices[6] = {0, 1, 2, 0, 2, 3};
        for (int i = 0; i < 6; i++) {
            meshData->indices[meshData->indexCount] = (meshData->indexCount + i);
            (meshData->indexCount)++;
        }
        }
        break;
        case TOP:{
        float faceVertices[4][3] = {
            {x, y + 1, z}, {x, y + 1, z + 1}, {x + 1, y + 1, z + 1}, {x + 1, y + 1, z}
        };
    
        for (int i = 0; i < 4; i++) {
            meshData->vertices[meshData->vertexCount * 3 + 0] = faceVertices[i][0];
            meshData->vertices[meshData->vertexCount * 3 + 1] = faceVertices[i][1];
            meshData->vertices[meshData->vertexCount * 3 + 2] = faceVertices[i][2];

            meshData->texCoords[meshData->vertexCount * 2 + 0] = (i == 1 || i == 2) ? 1.0f : 0.0f;
            meshData->texCoords[meshData->vertexCount * 2 + 1] = (i == 2 || i == 3) ? 1.0f : 0.0f;
    
            (meshData->vertexCount)++;
        }
    
        unsigned int faceIndices[6] = {0, 1, 2, 0, 2, 3};
        for (int i = 0; i < 6; i++) {
            meshData->indices[meshData->indexCount] = (meshData->indexCount + i);
            (meshData->indexCount)++;
        }}
        break;
        case BOTTOM:{
        float faceVertices[4][3] = {
            {x, y, z}, {x + 1, y, z}, {x + 1, y, z + 1}, {x, y, z + 1}
        };
    
        for (int i = 0; i < 4; i++) {
            meshData->vertices[meshData->vertexCount * 3 + 0] = faceVertices[i][0];
            meshData->vertices[meshData->vertexCount * 3 + 1] = faceVertices[i][1];
            meshData->vertices[meshData->vertexCount * 3 + 2] = faceVertices[i][2];

            meshData->texCoords[meshData->vertexCount * 2 + 0] = (i == 1 || i == 2) ? 1.0f : 0.0f;
            meshData->texCoords[meshData->vertexCount * 2 + 1] = (i == 2 || i == 3) ? 1.0f : 0.0f;
    
            (meshData->vertexCount)++;
        }
    
        unsigned int faceIndices[6] = {0, 1, 2, 0, 2, 3};
        for (int i = 0; i < 6; i++) {
            meshData->indices[meshData->indexCount] = (meshData->indexCount + i);
            (meshData->indexCount)++;
        }}
        break;
        case FRONT:{
        float faceVertices[4][3] = {
            {x, y, z + 1}, {x + 1, y, z + 1}, {x + 1, y + 1, z + 1}, {x, y + 1, z + 1}
        };
    
        for (int i = 0; i < 4; i++) {
            meshData->vertices[meshData->vertexCount * 3 + 0] = faceVertices[i][0];
            meshData->vertices[meshData->vertexCount * 3 + 1] = faceVertices[i][1];
            meshData->vertices[meshData->vertexCount * 3 + 2] = faceVertices[i][2];

            meshData->texCoords[meshData->vertexCount * 2 + 0] = (i == 1 || i == 2) ? 1.0f : 0.0f;
            meshData->texCoords[meshData->vertexCount * 2 + 1] = (i == 2 || i == 3) ? 1.0f : 0.0f;
    
            (meshData->vertexCount)++;
        }
    
        unsigned int faceIndices[6] = {0, 1, 2, 0, 2, 3};
        for (int i = 0; i < 6; i++) {
            meshData->indices[meshData->indexCount] = (meshData->indexCount + i);
            (meshData->indexCount)++;
        }}
        break;
        case BACK:{
        float faceVertices[4][3] = {
            {x + 1, y, z}, {x, y, z}, {x, y + 1, z}, {x + 1, y + 1, z}
        };
    
        for (int i = 0; i < 4; i++) {
            meshData->vertices[meshData->vertexCount * 3 + 0] = faceVertices[i][0];
            meshData->vertices[meshData->vertexCount * 3 + 1] = faceVertices[i][1];
            meshData->vertices[meshData->vertexCount * 3 + 2] = faceVertices[i][2];

            meshData->texCoords[meshData->vertexCount * 2 + 0] = (i == 1 || i == 2) ? 1.0f : 0.0f;
            meshData->texCoords[meshData->vertexCount * 2 + 1] = (i == 2 || i == 3) ? 1.0f : 0.0f;
    
            (meshData->vertexCount)++;
        }
    
        unsigned int faceIndices[6] = {0, 1, 2, 0, 2, 3};
        for (int i = 0; i < 6; i++) {
            meshData->indices[meshData->indexCount] = (meshData->indexCount + i);
            (meshData->indexCount)++;
        }}
        break;
    }
}

bool isFaceBlocked(Chunk* chunk, int x, int y, int z){
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
        return true;
    return !chunk->voxels[x][y][z].isActive;
}

void generateChunkMesh(Chunk* chunk, MeshData* meshData){
    meshData->vertices = malloc(MAX_VERTICES * sizeof(float));
    meshData->texCoords = malloc(MAX_VERTICES * sizeof(float));
    meshData->indices = malloc(MAX_INDICES * sizeof(float));

    int vertexCount = 0;
    int indexCount = 0;
    
    for(int x = 0; x < CHUNK_SIZE; x++){
        for(int y = 0; y < CHUNK_SIZE; y++){
            for(int z = 0; z < CHUNK_SIZE; z++){
                if(chunk->voxels[x][y][z].isActive){
                    if(isFaceBlocked(chunk, x + 1, y, z)){
                        addFaceData(RIGHT, meshData, x, y, z);
                    }
                    if(isFaceBlocked(chunk, x - 1, y, z)){
                        addFaceData(LEFT, meshData, x, y, z);
                    }
                    if(isFaceBlocked(chunk, x, y + 1, z)){
                        addFaceData(TOP, meshData, x, y, z);
                    }
                    if(isFaceBlocked(chunk, x, y - 1, z)){
                        addFaceData(BOTTOM, meshData, x, y, z);
                    }
                    if(isFaceBlocked(chunk, x, y, z + 1)){
                        addFaceData(FRONT, meshData, x, y, z);
                    }
                    if(isFaceBlocked(chunk, x, y, z - 1)){
                        addFaceData(BACK, meshData, x, y, z);
                    }
                }
            }
        }
    }

    
}

void drawMesh(Mesh* mesh, Texture2D textureAtlas) {
    rlPushMatrix();  // Save current transformation matrix
    rlLoadIdentity();  // Reset transformation matrix

    // Bind the texture atlas
    rlEnableTexture(textureAtlas.id);
    //rlBindTexture(textureAtlas.id);
    // Draw the mesh using Raylib’s rlgl (low-level OpenGL rendering)
    rlBegin(RL_TRIANGLES);
    rlRenderBatch batch = { 0 };
    for (int i = 0; i < *mesh->indices; i++) {
        int index = mesh->indices[i];
        
        // Set the vertex position
        rlVertex3f(mesh->vertices[index * 3 + 0], mesh->vertices[index * 3 + 1], mesh->vertices[index * 3 + 2]);

        // Set the texture coordinates
        rlTexCoord2f(mesh->texcoords[index * 2 + 0], mesh->texcoords[index * 2 + 1]);
        rlDrawRenderBatch(&batch);
    }

    //DrawMesh(*mesh, (Material){0}, MatrixIdentity());
    rlEnd();

    rlPopMatrix();  // Restore transformation matrix
}

Texture2D loadTextureAtlas(const char* path) {
    Texture2D textureAtlas = LoadTexture(path);  // Load texture atlas
    return textureAtlas;
}





void createChunks(World* world, Color* palette) {
    int colorIndex = 0;

    for (int chunkX = 0; chunkX < WORLD_SIZE; chunkX++) {
        for (int chunkY = 0; chunkY < WORLD_SIZE; chunkY++) {
            for (int chunkZ = 0; chunkZ < WORLD_SIZE; chunkZ++) {
                int index = chunkX * (WORLD_SIZE * WORLD_SIZE) + chunkY * WORLD_SIZE + chunkZ;

                Color color = palette[colorIndex];
                colorIndex = (colorIndex + 1) % 6;

                for (int x = 0; x < CHUNK_SIZE; x++) {
                    for (int y = 0; y < CHUNK_SIZE; y++) {
                        for (int z = 0; z < CHUNK_SIZE; z++) {
                            world->chunks[index].voxels[x][y][z].position = (Vector3){x + chunkX * CHUNK_SIZE, y + chunkY * CHUNK_SIZE, z + chunkZ * CHUNK_SIZE};
                            world->chunks[index].voxels[x][y][z].color = color;
                            world->chunks[index].voxels[x][y][z].isActive = true;
                        }
                    }
                }
            }
        }
    }
}

int main(void){
    Vector2 screenSize = {800, 600};
    World* world = (World *)malloc(sizeof(World));
    Mesh* meshes = (Mesh *)malloc(sizeof(Mesh) * WORLD_SIZE * WORLD_SIZE * WORLD_SIZE);
    if(world == NULL){
        WeGottaGo("Done goofed the whole world", 0);
    }

    if(meshes == NULL){
        WeGottaGo("Meshes made the memory go brrr", 0);
    }

    world->chunks = malloc((WORLD_SIZE * WORLD_SIZE * WORLD_SIZE) * sizeof(Chunk));
    Color* palette = (Color *)malloc(sizeof(Color) * 6);

    if(world->chunks == NULL){
        WeGottaGo("Done goofed yer chunks", 0);
    }

    if(palette == NULL){
        WeGottaGo("Done goofed them colors", 0);
    }
    
    palette[0] = BLUE;
    palette[1] = RED;
    palette[2] = GREEN;
    palette[3] = PURPLE;
    palette[4] = ORANGE;
    palette[5] = YELLOW;

    bool* tempFaces = (bool *)malloc(6 * sizeof(bool));

    if(tempFaces == NULL){
        WeGottaGo("goofy ahh faces", 0);
    }

    createChunks(world, palette);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenSize.x, screenSize.y, "Pete's Voxel Time");
    SetTargetFPS(144);
    
    Texture2D texturAtlas = loadTextureAtlas("X:/Repos/raylibGameBuild/Texture_01.png");

    for(int i = 0; i < WORLD_SIZE * WORLD_SIZE * WORLD_SIZE; i++){
        
        MeshData meshData = { 0 };
        generateChunkMesh(&world->chunks[i], &meshData);
        Mesh mesh = createRaylibMesh(&meshData);
        UploadMesh(&mesh, false);
        meshes[i] = mesh;
    }

    
    Camera camera = { 0 };
    camera.fovy = 67;
    camera.position = (Vector3){0.0f, 0.0f, 0.0f};
    camera.target = (Vector3){0.0f, 0.0f, 1.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;  

    DisableCursor();

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_FREE);

        if(IsWindowResized()){
            screenSize.x = GetScreenWidth();
            screenSize.y = GetScreenHeight();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
            
            // BeginMode3D(camera);
            
            //     for(int chunkX = 0; chunkX < WORLD_SIZE; chunkX++){
            //         for(int chunkY = 0; chunkY < WORLD_SIZE; chunkY++){
            //             for(int chunkZ = 0; chunkZ < WORLD_SIZE; chunkZ++){                           
            //                 for(int x = 0; x < CHUNK_SIZE; x++){
            //                     for(int y = 0; y < CHUNK_SIZE; y++){
            //                         for(int z = 0; z < CHUNK_SIZE; z++){     
            //                             int index = chunkX * (WORLD_SIZE * WORLD_SIZE) + chunkY * WORLD_SIZE + chunkZ;       
            //                             DrawCubeWires(world->chunks[index].voxels[x][y][z].position, 1, 1, 1, world->chunks[index].voxels[x][y][z].color);                                      
            //                         }
            //                     }
            //                 }
            //             }
            //         }
            //     }
            for(int i = 0; i < WORLD_SIZE * WORLD_SIZE * WORLD_SIZE; i++){
                drawMesh(&meshes[i], texturAtlas);
            }
             DrawGrid(10, 1);
            EndMode3D();
            
        DrawFPS(10, 10);
        EndDrawing();
    }

    free(world->chunks);
    free(world);
    free(palette);
    free(tempFaces);
    free(meshes);
    print("rlgl version: %2.2d", rlGetVersion());
    CloseWindow();
    return 0;
}
