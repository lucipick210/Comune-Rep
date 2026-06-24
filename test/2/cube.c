#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define W 80
#define H 24

char screen[H][W];

typedef struct {
    float x, y, z;
} Vec3;

void clear_screen() {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            screen[y][x] = ' ';
}

void put(int x, int y) {
    if (x >= 0 && x < W && y >= 0 && y < H)
        screen[y][x] = '#';
}

// proiecție 3D -> 2D
void project(Vec3 v, int *x, int *y) {
    float dist = 3;
    float z = v.z + 5;

    *x = (int)(v.x * dist / z * 20 + W / 2);
    *y = (int)(v.y * dist / z * 10 + H / 2);
}

// desenare linie (Bresenham simplu)
void line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (1) {
        put(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

int main() {
    float a = 0;

    Vec3 v[8] = {
        {-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},
        {-1,-1, 1},{1,-1, 1},{1,1, 1},{-1,1, 1}
    };

    int edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    while (1) {
        clear_screen();

        Vec3 r[8];

        // rotație + proiecție
        for (int i = 0; i < 8; i++) {
            float x = v[i].x;
            float y = v[i].y;
            float z = v[i].z;

            // rotație Y
            float xr = x * cos(a) - z * sin(a);
            float zr = x * sin(a) + z * cos(a);

            r[i].x = xr;
            r[i].y = y;
            r[i].z = zr;
        }

        // desenare muchii
        for (int i = 0; i < 12; i++) {
            int a1 = edges[i][0];
            int a2 = edges[i][1];

            int x0, y0, x1, y1;
            project(r[a1], &x0, &y0);
            project(r[a2], &x1, &y1);

            line(x0, y0, x1, y1);
        }

        printf("\033[H"); 

        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++)
                putchar(screen[y][x]);
            putchar('\n');
        }

        a += 0.05;
        usleep(30000);
    }
}