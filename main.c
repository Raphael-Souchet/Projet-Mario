#include "projet.h"

const int LARGEUR_MAP = 162;
const int SPAWN_X = 21;
const int SPAWN_Y = 15;
const int SPAWN_MORT_X = 21;
const int SPAWN_MORT_Y = 14;
const int MORT_Y = 17;

int main(int argc, char *argv[])
{
    const char *fichierOriginal = "Mario.txt";
    menuPrincipal(fichierOriginal);
    return 0;
}