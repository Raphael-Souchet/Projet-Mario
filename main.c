#include "projet.h"

const int LARGEUR_MAP = 163;
const int SPAWN_X = 21;
const int SPAWN_Y = 14;
const int SPAWN_MORT_X = 21;
const int SPAWN_MORT_Y = 14;
const int MORT_Y = 19;

int main(int argc, char *argv[])
{
    const char *fichierOriginal = "Mario.txt";
    menuPrincipal(fichierOriginal);
    return 0;
}