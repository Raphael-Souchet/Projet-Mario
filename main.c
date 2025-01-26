#include "projet.h"

int main() {
    const char *fichierOriginal = "Mario.txt";
    const char *fichierTemp = "temp.txt";

    copierFichier(fichierOriginal, fichierTemp);

    int choix = menu();

    Personnage perso = {21, 5, "Mario", 0};

    switch(choix) {
        case 1:
            jouer(fichierTemp, &perso);
            break;
        case 2:
            // fct score
            break;
        case 3:
            break;
    }
    
    return 0;
}