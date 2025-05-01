#include "projet.h"

void initialiser_gumbas(Carte *carte, Tab_gumba *tab_gumba)
{
    if (tab_gumba == NULL)
    {
        printf("Erreur: tab_gumba non valide\n");
        return;
    }

    if (tab_gumba->gumbas != NULL)
    {
        free(tab_gumba->gumbas);
        tab_gumba->gumbas = NULL;
        tab_gumba->count = 0;
    }

    int nbGumbas = 0;
    for (int y = 0; y < carte->hauteur; y++)
    {
        for (int x = 0; x < carte->largeur; x++)
        {
            if (carte->carte[y][x] == 'Q')
            {
                nbGumbas++;
            }
        }
    }

    if (nbGumbas > 0)
    {
        tab_gumba->count = nbGumbas;
        tab_gumba->gumbas = malloc(nbGumbas * sizeof(Gumba));

        if (tab_gumba->gumbas == NULL)
        {
            printf("Erreur: allocation de memoire pour les gumbas echouee\n");
            tab_gumba->count = 0;
            return;
        }

        int index = 0;
        for (int y = 0; y < carte->hauteur; y++)
        {
            for (int x = 0; x < carte->largeur; x++)
            {
                if (carte->carte[y][x] == 'Q')
                {
                    tab_gumba->gumbas[index].positionX = x;
                    tab_gumba->gumbas[index].positionY = y;
                    tab_gumba->gumbas[index].peut_avancer = 1;
                    tab_gumba->gumbas[index].peut_reculer = 1;
                    tab_gumba->gumbas[index].peut_tomber_devant = 0;
                    tab_gumba->gumbas[index].peut_tomber_derriere = 0;
                    tab_gumba->gumbas[index].dernier_deplacement = 1;
                    tab_gumba->gumbas[index].compteur = 0;
                    index++;
                }
            }
        }
        return;
    }

    tab_gumba->count = 4;
    tab_gumba->gumbas = malloc(tab_gumba->count * sizeof(Gumba));

    if (tab_gumba->gumbas == NULL)
    {
        printf("Erreur: allocation de memoire pour les gumbas echouee\n");
        tab_gumba->count = 0;
        return;
    }

    int tab_x[4] = {93, 98, 120, 150};
    int tab_y[4] = {14, 9, 8, 8};

    for (int i = 0; i < tab_gumba->count; i++)
    {
        tab_gumba->gumbas[i].positionX = tab_x[i];
        tab_gumba->gumbas[i].positionY = tab_y[i];
        tab_gumba->gumbas[i].peut_avancer = 1;
        tab_gumba->gumbas[i].peut_reculer = 0;
        tab_gumba->gumbas[i].peut_tomber_devant = 0;
        tab_gumba->gumbas[i].peut_tomber_derriere = 0;
        tab_gumba->gumbas[i].dernier_deplacement = 1;
        tab_gumba->gumbas[i].compteur = 0;

        carte->carte[tab_gumba->gumbas[i].positionY][tab_gumba->gumbas[i].positionX] = 'Q';
    }
}

void bouger_gumba(Carte *carte, Tab_gumba *tab_gumba)
{
    if (tab_gumba == NULL || tab_gumba->gumbas == NULL || tab_gumba->count <= 0)
    {
        return;
    }

    for (int i = 0; i < tab_gumba->count; i++)
    {
        carte->carte[tab_gumba->gumbas[i].positionY][tab_gumba->gumbas[i].positionX] = ' ';
        verifier_collision_gumba(carte, &tab_gumba->gumbas[i]);

        if (tab_gumba->gumbas[i].dernier_deplacement)
        {
            if (!tab_gumba->gumbas[i].peut_tomber_devant && tab_gumba->gumbas[i].peut_avancer)
            {
                tab_gumba->gumbas[i].positionX++;
            }
            else
            {
                tab_gumba->gumbas[i].positionX--;
                tab_gumba->gumbas[i].dernier_deplacement = 0;
            }
        }
        else
        {
            if (!tab_gumba->gumbas[i].peut_tomber_derriere && tab_gumba->gumbas[i].peut_reculer)
            {
                tab_gumba->gumbas[i].positionX--;
            }
            else
            {
                tab_gumba->gumbas[i].positionX++;
                tab_gumba->gumbas[i].dernier_deplacement = 1;
            }
        }

        carte->carte[tab_gumba->gumbas[i].positionY][tab_gumba->gumbas[i].positionX] = 'Q';
    }
}



