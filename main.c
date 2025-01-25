#include "projet.h"

int main() {
    FILE *fichier = fopen("Mario.txt", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier Mario.txt\n");
        return 1;
    }

    int choix = menu();
    int positionJoueur = 55; 
    switch(choix) {
        case 1:
            afficherPaysage(fichier, positionJoueur);
            break;
        case 2:
            // fct score
            break;
        case 3:
            break;
    }

    fclose(fichier);
    return 0;
}