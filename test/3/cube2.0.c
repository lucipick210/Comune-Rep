#include <stdio.h>   // For printf() and putchar()
#include <math.h>    // For sin() and cos()
#include <string.h>  // For memset()

// Rotation angles of the cube
float A, B, C;

// Z-buffer stores which point is closer to the camera
float zBuffer[1760];

// Screen buffer that contains the characters to display
char buffer[1760];

// Terminal dimensions
int width = 80;
int height = 22;

// Distance between the camera and the cube
float distanceFromCam = 100;

// Projection scaling factor
float K1 = 40;

// Half of the cube side length
int cubeWidth = 20;

// Calculate the rotated X coordinate
float calculateX(int i, int j, int k){
    return j * sin(A) * sin(B) * cos(C) -
           k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) +
           k * sin(A) * sin(C) +
           i * cos(B) * cos(C);
}

// Calculate the rotated Y coordinate
float calculateY(int i, int j, int k){
    return j * cos(A) * cos(C) +
           k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) +
           k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

// Calculate the rotated Z coordinate
float calculateZ(int i, int j, int k){
    return k * cos(A) * cos(B) -
           j * sin(A) * cos(B) +
           i * sin(B);
}

// Draw a point on one face of the cube
void calculateForSurface(float cubeX, float cubeY, float cubeZ, char ch)
{
    // Apply 3D rotation
    float x = calculateX(cubeX, cubeY, cubeZ);
    float y = calculateY(cubeX, cubeY, cubeZ);
    float z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    // One over z (used for perspective projection)
    float ooz = 1 / z;

    // Project the 3D point onto the 2D screen
    int xp = (int)(width / 2 + K1 * ooz * x * 2);
    int yp = (int)(height / 2 + K1 * ooz * y);

    // Convert 2D coordinates to buffer index
    int idx = xp + yp * width;

    // Check if the point is inside the screen
    if(idx >= 0 && idx < width * height)
    {
        // Draw the point only if it is closer to the camera
        if(ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main()
{
    // Clear the terminal screen once
    printf("\x1b[2J");

    // Main animation loop
    while(1)
    {
        // Fill the screen buffer with spaces
        memset(buffer, ' ', width * height);

        // Reset the z-buffer
        memset(zBuffer, 0, width * height * sizeof(float));

        // Iterate through all points on the cube faces
        for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += 0.6)
        {
            for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += 0.6)
            {
                // Front face
                calculateForSurface(cubeX, cubeY, -cubeWidth, '#');

                // Right face
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');

                // Left face
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');

                // Back face
                calculateForSurface(-cubeX, cubeY, cubeWidth, '@');

                // Bottom face
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');

                // Top face
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        // Move the cursor to the top-left corner
        // so the animation does not scroll the terminal
        printf("\x1b[H");

        // Print the screen buffer
        for(int k = 0; k < width * height; k++)
        {
            putchar(k % width ? buffer[k] : '\n');
        }

        // Update rotation angles
        A += 0.05;
        B += 0.05;
        C += 0.01;
    }

    return 0;
}