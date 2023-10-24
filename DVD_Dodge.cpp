// Libraries //
// --> "" External libraries
// --> <> C / C++ libraries
#include "raylib.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>

// So that std:: can be omited //
using namespace std;

// Screen size //
int x = 1200, y = 800;
Color colors[] = {(Color){0,128,128,255},(Color){128,0,0,255},(Color){0,128,0,255},(Color){0,0,128,255},(Color){128,0,128,255}};
    
// Enemy Struct
struct Ball{
    int x; // Current Position
    int y; // Current Position
    int radius; // Size
    int speed; // Speed
    int bounces; // No. walls hit
    int x_neg_pos; // Current Horizontal Movement Direction
    int y_neg_pos; // Current Vertical Movement Direction
    bool active; // Is this ball active?
    Color color; // Color of DvD
    int lastColor; // Previous Color of DvD
};

// Reset Enemies to starting State //
void ResetAllBalls(Ball balls[]){
    (balls[0]).x = (rand() %  2) ? x/4 : 3*x/4;
    (balls[0]).y = y/4;
    (balls[0]).radius = 8*(int)(3 + (rand() %  3)) + 2;
    (balls[0]).speed = 1;
    (balls[0]).x_neg_pos = ((1 + (rand() % 2)) % 2) ? 1 : -1;
    (balls[0]).y_neg_pos = ((1 + (rand() % 2)) % 2) ? 1 : -1;
    (balls[0]).bounces = 0;
    (balls[0]).active = true;
    (balls[0]).color = BLACK;
    (balls[0]).lastColor = 0;
    for (int i = 1; i < 10; i++)
    {
        (balls[i]).x = (rand() %  2) ? x/4 : 3*x/4;
        (balls[i]).y = y/4;
        (balls[i]).radius = 8*(int)(3 + (rand() %  3)) + 2;
        (balls[i]).speed = 1;
        (balls[i]).x_neg_pos = ((1 + (rand() % 2)) % 2) ? 1 : -1;
        (balls[i]).y_neg_pos = ((1 + (rand() % 2)) % 2) ? 1 : -1;
        (balls[i]).bounces = 0;
        (balls[i]).active = false;
        (balls[i]).color = BLACK;
        (balls[0]).lastColor = 0;
    }
};

// Check all Player/Enemy Collisions //
bool CheckAllBalls(Ball balls[], Rectangle player){
    for (int i = 0; i < 10; i++)
    {
        if(CheckCollisionCircleRec({(float)(balls[i]).x, (float)(balls[i]).y}, (float)(balls[i]).radius, player)){
            if((balls[i]).active) return true;
        }
    }
    return false;
};

// Update Player position within bounds //
Rectangle UpdatePlayerPositions(Rectangle player, Texture2D sprite){
    DrawRectangleRec(player, RED);
    Rectangle frameRec = {0.0f, 0.0f, (float)sprite.width/3, (float)sprite.height};
    if (IsKeyDown(KEY_D) && player.x < x-player.width)
    {   
        frameRec.x = (float)sprite.width/3;
        DrawTexturePro(sprite, frameRec, player, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        player.x += 10;
        return player;
    }
    if (IsKeyDown(KEY_A) && player.x > 0)
    {
        frameRec.x = 0.0f;
        DrawTexturePro(sprite, frameRec, player, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        player.x -= 10;
        return player;
    }
    
    frameRec.x = (float)2*(float)sprite.width/3;
    DrawTexturePro(sprite, frameRec, player, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    return player;
}

// Update all Enemy positions and bounces //
void UpdateAllCirclesPositions(Ball balls[], Texture2D sprite, Sound sound){
    Rectangle frameRec = {0.0f, 0.0f, (float)sprite.width, (float)sprite.height};
    for (int i = 0; i < 10; i++)
    {
        if ((balls[i]).active){ 
            DrawCircle((balls[i]).x, (balls[i]).y, (balls[i]).radius, (Color){255,255,255,50});
            Rectangle enemy = {(float)(balls[i]).x, (float)(balls[i]).y, (float)(balls[i]).radius*1.4f, (float)(balls[i]).radius*1.4f};
            DrawTexturePro(sprite, frameRec, enemy, (Vector2){(float)(balls[i]).radius*1.42f/2.0f, (float)(balls[i]).radius*1.25f/2.0f}, 0.0f, (balls[i]).color);
            (balls[i]).y += (balls[i]).y_neg_pos * (balls[i]).speed;
            (balls[i]).x += (balls[i]).x_neg_pos * (balls[i]).speed;
            if((balls[i]).y < (balls[i]).radius || (balls[i]).y > y-(balls[i]).radius)
            {
                PlaySound(sound);
                (balls[i]).y_neg_pos = -(balls[i]).y_neg_pos;
                (balls[i]).bounces++;
                (balls[i]).speed = (int)round(log(pow((balls[i]).bounces,2)))+1;
                int temp_color = (int)(rand() % (int)(sizeof(colors)/sizeof(int)));
                while(temp_color == (balls[i]).lastColor){
                    temp_color = (int)(rand() % (int)(sizeof(colors)/sizeof(int)));
                }
                (balls[i]).color = colors[temp_color];
                (balls[i]).lastColor = temp_color;
            }
            if((balls[i]).x < (balls[i]).radius || (balls[i]).x > x-(balls[i]).radius)
            {
                PlaySound(sound);
                (balls[i]).x_neg_pos = -(balls[i]).x_neg_pos;
                (balls[i]).bounces++;
                (balls[i]).speed = (int)round(log(pow((balls[i]).bounces,2)))+1;
                int temp_color = (int)(rand() % (int)(sizeof(colors)/sizeof(int)));
                while(temp_color == (balls[i]).lastColor){
                    temp_color = (int)(rand() % (int)(sizeof(colors)/sizeof(int)));
                }
                (balls[i]).color = colors[temp_color];
                (balls[i]).lastColor = temp_color;
            }
        }
    }
    return;
};

//  //
string CurrentBounces(Ball balls[]){
    int total_bounces = 0;
    for(int i = 0; i < 10; i++){
        total_bounces += (balls[i]).bounces;
    }
    return to_string(total_bounces);
};

int main(){
    // - Set Up Window -
    InitWindow(x,y,"DVD Dodge");
    InitAudioDevice();
    Music bgsfx = LoadMusicStream((string(GetWorkingDirectory()) + "\\Assets\\Audio\\BackgroundNoise.wav").c_str());
    PlayMusicStream(bgsfx);
    string windowIconPath = string(GetWorkingDirectory()) + "\\Assets\\Images\\DVD_Icon.png";
    Image windowIcon = LoadImage(windowIconPath.c_str());
    ImageColorTint(&windowIcon, DARKGRAY);
    SetWindowIcon(windowIcon); 
    int bg_counter = 0;
    Texture2D bg = LoadTextureFromImage(GenImageWhiteNoise(x/2, y/2, 0.6f));
    string gameTitlePath = string(GetWorkingDirectory()) + "\\Assets\\Images\\DVD_Dodge_Title.png";
    Texture2D gameTitle = LoadTextureFromImage(LoadImage(gameTitlePath.c_str()));
    SetExitKey(0);
    //  ----------------

    Rectangle player = {x/2.0f, y/2.0f, 32.0f, 32.0f};
    bool gameOver{false}, gameStart{false};
    double timer{0.0};
    char record[50] = "";
    string playerPath = string(GetWorkingDirectory()) + "\\Assets\\Images\\SpriteDVD.png";
    Texture2D playerSprite = LoadTextureFromImage(LoadImage(playerPath.c_str()));

    Ball activeBalls[10] = {
        {0,0,0,0,0,0,0,false,WHITE,0},
        {0,0,0,0,0,0,0,false,WHITE,0},
        {0,0,0,0,0,0,0,false,WHITE,0},
        {0,0,0,0,0,0,0,false,WHITE,0},
        {0,0,0,0,0,0,0,false,WHITE,0},
        {0,0,0,0,0,0,0,false,WHITE,0},
        {0,0,0,0,0,0,0,false,WHITE,0},
        {0,0,0,0,0,0,0,false,WHITE,0},
        {0,0,0,0,0,0,0,false,WHITE,0},
        {0,0,0,0,0,0,0,false,WHITE,0},
    };
    Texture2D dvdLogo = LoadTextureFromImage(windowIcon);
    ResetAllBalls(activeBalls);
    Sound fxWallHit = LoadSound((string(GetWorkingDirectory()) + "\\Assets\\Audio\\HitSE.wav").c_str());
    SetTargetFPS(60);  
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgsfx);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureEx(bg, (Vector2){0,0}, 0.0f, 2.0f, (Color) {255, 255, 255, 80});
        if(gameStart){
            if(gameOver)
            {
                DrawRectangleRec((Rectangle){(float)x/4+12, (float)y/6, (float)2*x/4, (float)4*y/6}, (Color){255,255,255,90});
                DrawText("GAME OVER", x/2.0f - MeasureTextEx(GetFontDefault(), "GAME OVER", 60.0f, 1.0f).x/2.0f, y/4, 60.0f, RED);
                sprintf(record,"%.2f sec.", timer);
                DrawText(record, x/2.0f - MeasureTextEx(GetFontDefault(), record, 48.0f, 1.0f).x/2.0f, y/2.0f-50, 48.0f, (Color){128,0,128,255});
                DrawText((CurrentBounces(activeBalls) + " bounces").c_str(), x/2.0f - MeasureTextEx(GetFontDefault(), (CurrentBounces(activeBalls) + " bounces").c_str(), 48.0f, 1.0f).x/2.0f, y/2.0f+50, 48.0f, (Color){128,0,128,255});
                DrawText("Press Enter To Try Again", x/2 - MeasureTextEx(GetFontDefault(), "Press Enter To Try Again", 32.0f, 1.0f).x/2.0f, 3*y/4.0f, 32.0f, GREEN);
                if(IsKeyPressed(KEY_ENTER))
                {
                    gameOver = false;
                    timer = GetTime();

                    // Reset All
                    player.x = x/2;
                    player.y = y/2;                    
                    ResetAllBalls(activeBalls);
                }
            }
            else
            {
                DrawText(CurrentBounces(activeBalls).c_str(), (int)11*x/12, (int)y/12, 32, GREEN);
                double temp = GetTime();
                if ((int)floor(temp-timer)%5 ==0 && temp-timer<50){
                    activeBalls[(int)floor((int)floor(temp-timer)/5)].active = true;
                }
                if(CheckAllBalls(activeBalls, player))
                {
                    gameOver = true;
                    timer = GetTime() - timer;
                }
                else
                {
                    player = UpdatePlayerPositions(player, playerSprite);
                    UpdateAllCirclesPositions(activeBalls, dvdLogo, fxWallHit);
                }
            }
        } 
        else
        {
            DrawTexturePro(gameTitle, (Rectangle){ 0.0f, 0.0f, (float)gameTitle.width, (float)gameTitle.height}, (Rectangle){ (float)x/2, (float)y/2, (float)gameTitle.width*0.5f, (float)gameTitle.height*0.5f }, (Vector2){(float)gameTitle.width*0.5f*0.5f,(float)gameTitle.height*0.5f*0.5f}, 0.0f, WHITE);
            DrawText("Press Enter To Play", x/2 - 180, y/2 + 200, 32, GREEN);
            if(IsKeyPressed(KEY_ENTER))
            {
                gameStart = true;
                timer = GetTime();
            }
        }
        EndDrawing();
        if (bg_counter > 1) {
            UnloadTexture(bg);
            bg = LoadTextureFromImage(GenImageWhiteNoise(x/2, y/2, 0.6f));
            bg_counter = 0;
        } else {
            bg_counter++;
        }
    }
}



/*
    Multi line comment
    @brief Example function that doubles input value
    @tparam Here int is an integrer
    @param i is number to enter
    @return two times i
    @deprecated This is an obsolete comment
bool collision
*/