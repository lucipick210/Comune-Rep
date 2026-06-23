#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const char *p;
double expresie();

double numar(){
    while (*p == ' ')p++;
    double rezultat = strtod(p,(char**)&p);

    while (*p == ' ')p++;
    return rezultat;
}

double factor(){
    while (*p == ' ')p ++;
    if (*p == '(')
    {
        double rezultat = expresie();
        if (*p == ')')
            p++;
        return rezultat;
    }
    
    return numar();
}

double termen(){
    double rezultat = factor();
    while(1){
        while(*p == ' ') p++;
        if(*p == '*'){
            p++;
            rezultat *= factor();
        }
        else if (*p == '/'){
            p++;
            rezultat /= factor();
        }
        else{
            break;
        }
    }
    return rezultat;
}

double expresie(){
    double rezultat = termen();

    while(1){
        while(*p== ' ') p++;

        if(*p == '+'){
            p++;
            rezultat += termen();
        }
        else if (*p == '-'){
            p++;
            rezultat -= termen();
        }
        else{
            break;
        }
    }
    return rezultat;
}

double eval(const char *text){
    p = text;
    return expresie();
}

int main(){
    char input[256];
    printf("Expresion: ");
    fgets(input, sizeof(input),stdin);
    printf("Rezultat = %.0f\n",eval(input));
    return 0;
}
