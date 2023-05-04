#include "raylib.h"
#include <iostream>
#include <string>
#include <bits/stdc++.h>

void shuffle_array(int arr[], int n)
{

    // To obtain a time-based seed
    unsigned seed = 0;

    // Shuffling our array using random_shuffle
    std::random_shuffle(arr, arr + n);
}

int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1250;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Memory Tilez BOI");
    InitAudioDevice();

    Sound ende = LoadSound("./sound/ende.mp3");
    Sound clickFX = LoadSound("./sound/click.mp3");
    Texture2D Tex[9];
    Texture2D TexD[4][4];

    Tex[0] = LoadTexture("./aset/1.png");
    Tex[1] = LoadTexture("./aset/2.png");
    Tex[2] = LoadTexture("./aset/3.png");
    Tex[3] = LoadTexture("./aset/4.png");
    Tex[4] = LoadTexture("./aset/5.png");
    Tex[5] = LoadTexture("./aset/6.png");
    Tex[6] = LoadTexture("./aset/7.png");
    Tex[7] = LoadTexture("./aset/8.png");
    Tex[8] = LoadTexture("./aset/9.png");

    int arr[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int prase[4][4];
    int tiles[8];
    int matrix[16];
    int pprase[2];
    int g = 0;

    shuffle_array(arr, 9);

    for (int i = 0; i < 8; i++)
    {
        tiles[i] = arr[i];
    }
    for (int i = 0; i < 16; i++)
    {
        if (i < 8)
        {
            matrix[i] = tiles[i];
        }
        else
        {
            matrix[i] = tiles[i - 8];
        }
    }

    shuffle_array(matrix, 16);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            prase[i][j] = matrix[g];
            g++;
        }
    }

    int k = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            TexD[i][j] = Tex[matrix[k]];
            k++;
        }
    }

    // stanje pokrivenosti (sve su nule na pocetku), pravimo za fiksni niz.

    int btnState[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            btnState[i][j] = 0;
        }
    }

    float frameWidth = 150; // na pola iseces da prikazes prvu polovinu teksture sourcerec.x = btnState * frameWidth;

    Rectangle sourceRec[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            sourceRec[i][j] = {0, 0, 150, 150};
        }
    }

    Rectangle dodir[4][4]; // nacrtasmo nevidljivi pravougaonik na koj je moguce kliknuti

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            dodir[i][j] = {i * 170.0f + 300.0f, j * 170.0f + 100.0f, 150, 150};
        }
    }

    Vector2 mousePoint = {0.0f, 0.0f};

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    float timer = 0.0f;
    int frame = 1;
    int clicks = 0;
    int comp = 0;
    int once = 0;
    bool endState = false;
    int endState1 = 0;
    int *ptr[2];
    Rectangle *ptrsrc[2];
    int a = 0;
    int b = 0;

    // Main game loop
    while (!WindowShouldClose())
    {
        /*  mousePoint = GetMousePosition();
         timer += GetFrameTime(); */

        // Provera random generatora niza

        if (once < 1)
        {

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (j % 4 == 0)
                        std::cout << std::endl;
                    std::cout << " " << prase[i][j];
                }
            }

            std::cout << std::endl;

            once++;
        }

        mousePoint = GetMousePosition();
        timer += GetFrameTime();

        if (timer >= 0.01667f)
        {
            timer = 0.0f;
            frame += 1;
        }

        // Check button state

        // if (frame<20){

        if (clicks < 2)
        {

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (!endState)
                    {
                        if (CheckCollisionPointRec(mousePoint, dodir[i][j]))
                        {

                            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                            {
                                PlaySound(clickFX);
                                btnState[i][j] = 1;
                                ptr[a] = &btnState[i][j];
                                ptrsrc[a] = &dodir[i][j];
                                pprase[comp] = prase[j][i];
                                clicks++;
                                comp++;
                                a++;
                                // std::cout << "Prase :" << pprase[comp];
                                // std::cout << std::endl;
                                frame = 0;
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                sourceRec[i][j].x = btnState[i][j] * frameWidth;
            }
        }

        if (frame > 20)
        {
            if (clicks == 2)
            {
                if (pprase[0] != pprase[1])
                {
                    *ptr[0] = 0;
                    *ptr[1] = 0;
                    clicks = 0;
                    comp = 0;
                    a = 0;
                }
                if (pprase[0] == pprase[1])
                {
                    *ptrsrc[0] = {0, 0, 0, 0};
                    *ptrsrc[1] = {0, 0, 0, 0};
                    clicks = 0;
                    comp = 0;
                    a = 0;
                }
                frame = 0;
            }
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                sourceRec[i][j].x = btnState[i][j] * frameWidth;
            }
        }

        // Endstate check
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                endState1 += btnState[i][j];
            }
        }

        if (endState1 == 16)
        {
            endState = true;
        }
        endState1 = 0;

        if (endState && b < 1)
        {
            PlaySound(ende);
            b++;
        }
        // Endstate check

        // Reset game

        if (IsKeyReleased(KEY_R))
        {
            shuffle_array(arr, 9);

            for (int i = 0; i < 8; i++)
            {
                tiles[i] = arr[i];
            }
            for (int i = 0; i < 16; i++)
            {
                if (i < 8)
                {
                    matrix[i] = tiles[i];
                }
                else
                {
                    matrix[i] = tiles[i - 8];
                }
            }

            shuffle_array(matrix, 16);

            g = 0;

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    prase[i][j] = matrix[g];
                    g++;
                }
            }

            int k = 0;

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    TexD[i][j] = Tex[matrix[k]];
                    k++;
                }
            }

            // hide all tiles
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    btnState[i][j] = 0;
                }
            }
            // reset hitboxes
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    dodir[i][j] = {i * 170.0f + 300.0f, j * 170.0f + 100.0f, 150, 150};
                }
            }
            // Debug
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (j % 4 == 0)
                        std::cout << std::endl;
                    std::cout << " " << prase[i][j];
                }
            }

            endState = false;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(WHITE);

        // documentation reminder
        // DrawTexture(Tex1, screenWidth/2-150, screenHeight/2-150, WHITE);
        // DrawTexture(Tex2, screenWidth/2-400, screenHeight/2-400, WHITE);

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                DrawTextureRec(TexD[j][i], sourceRec[i][j], (Vector2){i * 170.0f + 300.0f, j * 170.0f + 100.0f}, WHITE);
            }
        }

        DrawText("Press R for Restart!!!", 10, 20, 40, BLACK);

        if (endState)
        {
            DrawRectangle(screenWidth / 2 - 260, screenHeight / 2 - 110, 520, 220, BLACK);
            DrawRectangle(screenWidth / 2 - 250, screenHeight / 2 - 100, 500, 200, WHITE);
            DrawText("Imas mozak, nisi totalno autist, heheheh", screenWidth / 2 - 200, screenHeight / 2, 20, BLACK);
        }

        // DrawRectangle(500, 300, 150, 150, WHITE);
        // DrawTexture(Texture2D texture, int posX, int posY, Color tint);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}