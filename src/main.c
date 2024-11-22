#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>


#define GRID_WIDTH 32
#define GRID_HEIGHT 32
#define CELL_SIZE 16
#define SCREEN_WIDTH (GRID_WIDTH * CELL_SIZE)
#define SCREEN_HEIGHT (GRID_HEIGHT * CELL_SIZE)

int grid[GRID_HEIGHT + 1][GRID_WIDTH + 1] = {0};


void draw_msq(void);
int get_state(int a, int b, int c, int d);
bool is_valid_point(int x, int y);
void draw_grid(void);
void draw_edge(Vector2 mid_points[4], int state);

/* Line stroke for each point configuration within the square */
int line_table[16][4] = {
        { -1, -1, -1, -1 },
        { 2, 3, -1, -1 },
        { 1, 2, -1, -1 },
        { 1, 3, -1, -1 },
        { 0, 1, -1, -1 },
        { 0, 3, 1, 2 },
        { 0, 2, -1, -1 },
        { 0, 3, -1, -1 },
        { 0, 3, -1, -1 },
        { 0, 2, -1, -1 },
        { 0, 1, 2, 3 },
        { 0, 1, -1, -1 },
        { 1, 3, -1, -1 },
        { 1, 2, -1, -1 },
        { 2, 3, -1, -1 },
        { -1, -1, -1, -1 }
};


int main(void)
{
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "msq_demo");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
                /* add point to grid on mouse click */

                Vector2 mouse_pos = GetMousePosition();
                int grid_x = (int)(mouse_pos.x / CELL_SIZE);
                int grid_y = (int)(mouse_pos.y / CELL_SIZE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        if (is_valid_point(grid_x, grid_y))
                                grid[grid_y][grid_x] = 1;
                }
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                        if (is_valid_point(grid_x, grid_y))
                                grid[grid_y][grid_x] = 0;
                }

                BeginDrawing();
                ClearBackground(BLACK);
                draw_grid();
                draw_msq();
                EndDrawing();
        }
        UnloadTexture(grass_tex);
        CloseWindow();
        return 0;
}


void draw_msq(void)
{
        int y;
        int x;
        for (y = 0; y < GRID_HEIGHT - 1; y++) {
                for (x = 0; x < GRID_WIDTH - 1; x++) {
                        int i;
                        float pos_x = x * CELL_SIZE;
                        float pos_y = y * CELL_SIZE;
                        /* Vector midpoints on each square */
                        Vector2 a = { pos_x + CELL_SIZE * 0.5f, pos_y };
                        Vector2 b = { pos_x + CELL_SIZE, pos_y + CELL_SIZE * 0.5f };
                        Vector2 c = { pos_x + CELL_SIZE * 0.5f, pos_y + CELL_SIZE };
                        Vector2 d = { pos_x, pos_y + CELL_SIZE * 0.5f };

                        int state = get_state(grid[y][x], grid[y][x+1], grid[y+1][x+1], grid[y+1][x]);
                        Vector2 points[4] = { a, b, c, d };
                        draw_edge(points, state);
                }
        }
}


void draw_edge(Vector2 mid_points[4], int state)
{
        int i;
        for (i = 0; i < 4 && line_table[state][i] != -1; i += 2)
                DrawLineEx(mid_points[line_table[state][i]], mid_points[line_table[state][i+1]], 1.0f, GREEN);
}


int get_state(int a, int b, int c, int d)
{
        return a * 8 + b * 4 + c * 2 + d;
}


bool is_valid_point(int x, int y)
{
        bool x_valid = (x >= 0 && x <= GRID_WIDTH);
        bool y_valid = (y >= 0 && y <= GRID_HEIGHT);
        return (x_valid && y_valid);
}


void draw_grid(void)
{
        int x;
        int y;
        for (int y = 0; y <= GRID_HEIGHT; y++) {
                for (int x = 0; x <= GRID_WIDTH; x++) {
                        Vector2 point = {x * CELL_SIZE, y * CELL_SIZE};
                        DrawCircleV(point, 2, WHITE);
                        if (grid[y][x] == 1) {
                                DrawCircleV(point, 3, DARKGREEN);
                        }
                }
        }
}


