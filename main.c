#include "projet.h"

int main() {
    const char *fichierOriginal = "Mario.txt";
    const char *fichierTemp = "temp.txt";

    copierFichier(fichierOriginal, fichierTemp);

    Personnage perso = {21, 5, "Mario", 0, 0};
    
    menuPrincipal(fichierTemp);
    
    return 0;
}