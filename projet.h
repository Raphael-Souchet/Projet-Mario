#ifndef PROJET
#define PROJET

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

typedef struct {
    int positionX;
    int positionY;
} Personnage;

void copierFichier(const char *source, const char *destination);
void caracterePaysage(char caractereActuel);
void afficherPaysage(FILE *fichier, int *positionJoueur);
int menu();
void jouer(const char *fichierTemp, int posx, int posy);

#endif