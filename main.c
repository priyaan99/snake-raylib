#include "raylib.h"
#include <stdio.h> // sprintf

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define FONT_SIZE 50
#define BOX_SIZE 25
#define MAX_LEN 100

typedef struct {
    int len;
    Vector2 pos[MAX_LEN];
} Snake;

Vector2 prevPos[MAX_LEN];
Snake snake;
Vector2 speed;
Vector2 foodPos;
bool food;
int score;

typedef enum {
    OUT,
    PAUSE,
    PLAY,
} State;

State game_state;

void init();
void update();
void draw();

int main() 
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "dev");
    SetTargetFPS(12);

    init();

    while (!WindowShouldClose())
    {
        update();
        draw();
    }

    CloseWindow();

    return 0;
}

void init() 
{
    snake.len = 1;
    snake.pos[0].x = 0;
    snake.pos[0].y = 0;

    prevPos[0].x = 0;
    prevPos[0].y = 0;

    speed.x = BOX_SIZE;
    speed.y = 0;
    
    foodPos.x = 100;
    foodPos.y = 100;

    game_state = PLAY;
    score = 0;
}

void draw_grid() 
{
    Vector2 startPos = (Vector2){0, 0}; 
    Vector2 endPos = (Vector2){SCREEN_WIDTH, 0}; 

    for (int i = 0; i <= SCREEN_HEIGHT/BOX_SIZE; i++) 
    {
        DrawLineEx(startPos, endPos, 2, GRAY);
        startPos.y += BOX_SIZE;
        endPos.y += BOX_SIZE;
    }

    startPos.x = 0;
    startPos.y = 0;
    endPos.x = 0;
    endPos.y = SCREEN_HEIGHT;
    for (int i = 0; i <= SCREEN_WIDTH/BOX_SIZE; i++) 
    {
        DrawLineEx(startPos, endPos, 2, GRAY);
        startPos.x += BOX_SIZE;
        endPos.x += BOX_SIZE;
    }
}

void run_game() {
    for (int i = 0; i < snake.len; i++) {
        prevPos[i] = snake.pos[i];
    }

    // update head pos of snake based on input
    if (IsKeyPressed(KEY_A) && speed.x == 0) {
        speed.x = -BOX_SIZE;
        speed.y = 0;
    }

    if (IsKeyPressed(KEY_D) && speed.x == 0) {
        speed.x = BOX_SIZE;
        speed.y = 0;
    }

    if (IsKeyPressed(KEY_W) && speed.y == 0) {
        speed.x = 0;
        speed.y = -BOX_SIZE;
    }

    if (IsKeyPressed(KEY_S) && speed.y == 0) {
        speed.x = 0;
        speed.y = BOX_SIZE;
    }
    
    if (IsKeyPressed(KEY_SPACE)) {
        game_state = PAUSE;
    }

    snake.pos[0].x += speed.x;
    snake.pos[0].y += speed.y;


    // food collision

    if (foodPos.x == snake.pos[0].x && foodPos.y == snake.pos[0].y) {
        snake.len++;
        score++;
        food = true;
    }

    // self collision
    for (int i = 1; i < snake.len; i++) {
        if (snake.pos[0].x == snake.pos[i].x && snake.pos[0].y == snake.pos[i].y) {
            game_state = OUT;
        }
    }

    // wall collision
    if (snake.pos[0].x < 0 || snake.pos[0].x > SCREEN_WIDTH || snake.pos[0].y < 0 || snake.pos[0].y > SCREEN_HEIGHT) {
        game_state = OUT;
    }

    // regenerate food 

    if (food) {
        bool done = false;
        while (!done) {
            foodPos.x = GetRandomValue(0, SCREEN_WIDTH/BOX_SIZE-1) * BOX_SIZE;
            foodPos.y = GetRandomValue(0, SCREEN_HEIGHT/BOX_SIZE-1) * BOX_SIZE;

            done = true;
            for (int i = 0; i < snake.len; i++) {
                if (foodPos.x == snake.pos[i].x && foodPos.y == snake.pos[i].y) {
                    done = false;
                }
            }
        }

        food = false;
    }

    // copy pos
    for (int i = 0; i < snake.len-1; i++) {
        snake.pos[i+1] = prevPos[i];
    }
}

void update()
{
    switch (game_state) {
        case PAUSE:
            if (IsKeyPressed(KEY_SPACE)) {
                game_state = PLAY;
            }
            break;
        case OUT:
            if (IsKeyPressed(KEY_ENTER)) {
                init();
                game_state = PLAY;
            }
            break;
        case PLAY: run_game();
    }
}

void draw_game() {
    for (int i = 0; i < snake.len; i++) {
        DrawRectangle(snake.pos[i].x, snake.pos[i].y, BOX_SIZE, BOX_SIZE, BLUE);
    }
    DrawRectangle(foodPos.x, foodPos.y, BOX_SIZE, BOX_SIZE, RED);
    draw_grid();
}

void draw() 
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (game_state) {
        case PAUSE:
            draw_game();
            break;
        case OUT:
            DrawText("Out", GetScreenWidth()/2-MeasureText("Out", FONT_SIZE)/2, GetScreenHeight()/2-120, FONT_SIZE, RED);
            char text[100];
            sprintf(text, "Score :: %d", score);
            DrawText(text ,GetScreenWidth()/2-MeasureText(text, FONT_SIZE)/2, GetScreenHeight()/2, FONT_SIZE, RED);
            break;
        case PLAY: 
            draw_game();
            break;       
    }

    EndDrawing();
}

