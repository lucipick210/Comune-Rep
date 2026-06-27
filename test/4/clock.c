#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>

int main() {
    while (1) {
        time_t now = time(NULL);
        struct tm *local = localtime(&now);

        system("cls"); 
        printf("%02d:%02d:%02d\n",
               local->tm_hour,
               local->tm_min,
               local->tm_sec);

        Sleep(1000);
    }

    return 0;
}