#include "projet.h"

int main() {
    const char *fichierOriginal = "Mario.txt";
    const char *fichierTemp = "temp.txt";

    copierFichier(fichierOriginal, fichierTemp);    
    menuPrincipal(fichierTemp);
    
    return 0;
}