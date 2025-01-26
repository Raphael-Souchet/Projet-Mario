#include "projet.h"

int main() {
    const char *fichierOriginal = "Mario.txt";
    const char *fichierTemp = "temp.txt";

    copierFichier(fichierOriginal, fichierTemp);

    int choix = menu();
    int posx = 21; 
    int posy = 5;

    switch(choix) {
        case 1:
            jouer(fichierTemp, posx, posy);
            break;
        case 2:
            // fct score
            break;
        case 3:
            break;
    }
    
    return 0;
}