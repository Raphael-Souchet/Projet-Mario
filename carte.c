#include "projet.h"

Carte *chargerCarteEnMemoire(const char *fichierCarte)
{
    FILE *fichier = fopen(fichierCarte, "r");
    if (fichier == NULL)
    {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", fichierCarte);
        return NULL;
    }

    Carte *carte = malloc(sizeof(Carte));
    if (carte == NULL)
    {
        fclose(fichier);
        printf("Erreur d'allocation de memoire pour la carte\n");
        return NULL;
    }

    int hauteur = 0;
    int largeur = 0;
    char ligne[1024];

    while (fgets(ligne, sizeof(ligne), fichier))
    {
        hauteur++;
        int longLigne = strlen(ligne);
        if (ligne[longLigne - 1] == '\n')
        {
            longLigne--;
        }
        if (longLigne > largeur)
        {
            largeur = longLigne;
        }
    }

    carte->hauteur = hauteur;
    carte->largeur = largeur;

    carte->carte = malloc(hauteur * sizeof(char *));
    if (carte->carte == NULL)
    {
        fclose(fichier);
        free(carte);
        printf("Erreur d'allocation de memoire pour les lignes\n");
        return NULL;
    }

    for (int i = 0; i < hauteur; i++)
    {
        carte->carte[i] = malloc((largeur + 1) * sizeof(char));
        if (carte->carte[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(carte->carte[j]);
            }
            free(carte->carte);
            free(carte);
            fclose(fichier);
            printf("Erreur d'allocation de memoire pour une ligne\n");
            return NULL;
        }
        memset(carte->carte[i], ' ', largeur);
        carte->carte[i][largeur] = '\0';
    }

    rewind(fichier);
    int y = 0;
    while (fgets(ligne, sizeof(ligne), fichier) && y < hauteur)
    {
        int longLigne = strlen(ligne);
        if (ligne[longLigne - 1] == '\n')
        {
            ligne[longLigne - 1] = '\0';
            longLigne--;
        }
        strncpy(carte->carte[y], ligne, longLigne);
        y++;
    }

    fclose(fichier);
    return carte;
}

void libererCarte(Carte *carte)
{
    if (carte == NULL)
        return;

    for (int i = 0; i < carte->hauteur; i++)
    {
        free(carte->carte[i]);
    }
    free(carte->carte);
    free(carte);
}

int sauvegarderCarteVersFichier(Carte *carte, const char *fichierCarte)
{
    FILE *fichier = fopen(fichierCarte, "w");
    if (fichier == NULL)
    {
        printf("Erreur: Impossible d'ouvrir le fichier %s pour sauvegarder\n", fichierCarte);
        return 0;
    }

    for (int i = 0; i < carte->hauteur; i++)
    {
        fprintf(fichier, "%s\n", carte->carte[i]);
    }

    fclose(fichier);
    return 1;
}