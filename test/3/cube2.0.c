#include <stdio.h>
#include <math.h>
#include <string.h>

float A, B, C;

float zBuffer[1760];
char buffer[1760];

int width = 80;
int height = 22;

float distanceFromCam = 100;
float K1 = 40;
int cubeWidth = 20;

float calculateX(int i, int j, int k){
    return j * sin(A) * sin(B) * cos(C) -
           k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) +
           k * sin(A) * sin(C) +
           i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k){
    return j * cos(A) * cos(C) +
           k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) +
           k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k){
    return k * cos(A) * cos(B) -
           j * sin(A) * cos(B) +
           i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char ch)
{
    float x = calculateX(cubeX, cubeY, cubeZ);
    float y = calculateY(cubeX, cubeY, cubeZ);
    float z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    float ooz = 1 / z;

    int xp = (int)(width / 2 + K1 * ooz * x * 2);
    int yp = (int)(height / 2 + K1 * ooz * y);

    int idx = xp + yp * width;

    if(idx >= 0 && idx < width * height)
    {
        if(ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main()
{
    printf("\x1b[2J");

    while(1)
    {
        memset(buffer, ' ', width * height);
        memset(zBuffer, 0, width * height * sizeof(float));

        for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += 0.6)
        {
            for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += 0.6)
            {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '#');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '@');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        printf("\x1b[H");

        for(int k = 0; k < width * height; k++)
        {
            putchar(k % width ? buffer[k] : '\n');
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;
    }

    return 0;
}