#ifndef PROJET_H
#define PROJET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>


typedef struct {
    int positionX;
    int positionY;
    char nom[100];
    int en_saut;
    int score;
    int en_chute;
    int peut_monter;
    int peut_avancer;
    int peut_reculer;
} Personnage;


typedef struct {
    char nom[100];
    int score;
    char date[100];
} Sauvegarde;

void gerer_saut(FILE *fichier, Personnage *perso, int largeur, int direction);
void effacer_position(FILE *fichier, Personnage *perso, int largeur);
void mettre_position(FILE *fichier, Personnage *perso, int largeur);
int copierFichier(const char *source, const char *destination);
void deplacer_joueur(FILE *fichier, Personnage *perso, int largeur);
void caracterePaysage(char caractereActuel);
void afficherPaysage(FILE *fichier, int positionJoueur);
void menuPrincipal(const char *fichierTemp);
void jouer(const char *fichierTemp, Personnage *perso);
void sauvegarderPartie(Personnage *perso, FILE *fichier);
int chargerPartie(Personnage *perso);
void menuSauvegarde(Personnage *perso, FILE *fichier);
void resetScores();
void afficherScores();
void verifier_collision(FILE *fichier, Personnage* perso, int largeur);
void cacherCurseur();
char* creerNomFichierTemp(const char* nomJoueur);


#endif