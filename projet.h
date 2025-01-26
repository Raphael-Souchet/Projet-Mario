#ifndef PROJET
#define PROJET

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

typedef struct {
    int positionX;
    int positionY;
    char nom[100];
    int en_saut;
} Personnage;

void copierFichier(const char *source, const char *destination);
void deplacer_joueur(FILE *fichier, Personnage *perso, int largeur);
void caracterePaysage(char caractereActuel);
void afficherPaysage(FILE *fichier, int positionJoueur);
int menu();
void jouer(const char *fichierTemp, Personnage *perso);

#endif