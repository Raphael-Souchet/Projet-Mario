#include "projet.h"

const int LARGEUR_MAP = 162;
const int SPAWN_X = 117;
const int SPAWN_Y = 6;
const int SPAWN_MORT_X = 21;
const int SPAWN_MORT_Y = 14;
const int MORT_Y = 17;

int main()
{
    const char *fichierOriginal = "Mario.txt";
    menuPrincipal(fichierOriginal);
    return 0;
}