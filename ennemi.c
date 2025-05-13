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
        tab_gumba->compteur = 0;
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
                    index++;
                }
            }
        }
        return;
    }

    tab_gumba->count = 3;
    tab_gumba->gumbas = malloc(tab_gumba->count * sizeof(Gumba));

    if (tab_gumba->gumbas == NULL)
    {
        printf("Erreur: allocation de memoire pour les gumbas echouee\n");
        tab_gumba->count = 0;
        return;
    }

    int tab_x[4] = {74, 80, 120};
    int tab_y[4] = {14, 9, 8};

    for (int i = 0; i < tab_gumba->count; i++)
    {
        tab_gumba->gumbas[i].positionX = tab_x[i];
        tab_gumba->gumbas[i].positionY = tab_y[i];
        tab_gumba->gumbas[i].peut_avancer = 1;
        tab_gumba->gumbas[i].peut_reculer = 0;
        tab_gumba->gumbas[i].peut_tomber_devant = 0;
        tab_gumba->gumbas[i].peut_tomber_derriere = 0;
        tab_gumba->gumbas[i].dernier_deplacement = 1;

        carte->carte[tab_gumba->gumbas[i].positionY][tab_gumba->gumbas[i].positionX] = 'Q';
    }
}

void bouger_gumba(Carte *carte, Tab_gumba *tab_gumba)
{
    if (tab_gumba == NULL || tab_gumba->gumbas == NULL || tab_gumba->count <= 0)
    {
        return;
    }

    if (tab_gumba->compteur != 0 && tab_gumba->compteur <= 3) {
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
        tab_gumba->compteur = 0;
    }
    else {
        tab_gumba->compteur = 1;
    }
}

void initialiserPlante(Carte *carte, Tab_plante *tab_plante)
{
    if (tab_plante == NULL)
    {
        printf("Erreur: tab_plante non valide\n");
        return;
    }

    if (tab_plante->plantes != NULL)
    {
        free(tab_plante->plantes);
        tab_plante->plantes = NULL;
        tab_plante->count = 0;
    }

    int nbPlantes = 0;
    for (int y = 0; y < carte->hauteur; y++)
    {
        for (int x = 0; x < carte->largeur; x++)
        {
            if (carte->carte[y][x] == 'P')
            {
                nbPlantes++;
            }
        }
    }

    if (nbPlantes > 0)
    {
        tab_plante->count = nbPlantes;
        tab_plante->plantes = malloc(nbPlantes * sizeof(Plante));

        if (tab_plante->plantes == NULL)
        {
            printf("Erreur: allocation de memoire pour les plantes echouee\n");
            tab_plante->count = 0;
            return;
        }

        int index = 0;
        for (int y = 0; y < carte->hauteur; y++)
        {
            for (int x = 0; x < carte->largeur; x++)
            {
                if (carte->carte[y][x] == 'P')
                {
                    tab_plante->plantes[index].positionX = x;
                    tab_plante->plantes[index].positionY = y;
                    tab_plante->plantes[index].positionY_base = y; 
                    tab_plante->plantes[index].peut_monter = 1;
                    tab_plante->plantes[index].peut_descendre = 1;
                    tab_plante->plantes[index].compteur = 0;
                    tab_plante->plantes[index].etat_tige = 0;

                    if (x > 0 && x < carte->largeur - 1) {
                        carte->carte[y][x-1] = ']';
                        carte->carte[y][x] = 'u';
                        carte->carte[y][x+1] = '[';
                    }

                    index++;
                }
            }
        }
        return;
    }

    tab_plante->count = 2;
    tab_plante->plantes = malloc(tab_plante->count * sizeof(Plante));

    if (tab_plante->plantes == NULL)
    {
        printf("Erreur: allocation de mémoire pour les plantes échouée\n");
        tab_plante->count = 0;
        return;
    }

    int tab_x[2] = {88, 94};
    int tab_y[2] = {14, 9};

    for (int i = 0; i < tab_plante->count; i++)
    {
        if (tab_x[i] < 0 || tab_x[i] >= carte->largeur || tab_y[i] < 0 || tab_y[i] >= carte->hauteur)
        {
            printf("Position invalide pour la plante %d\n", i);
            continue;
        }

        tab_plante->plantes[i].positionX = tab_x[i];
        tab_plante->plantes[i].positionY = tab_y[i];
        tab_plante->plantes[i].positionY_base = tab_y[i]; 
        tab_plante->plantes[i].peut_monter = 1;
        tab_plante->plantes[i].peut_descendre = 1;
        tab_plante->plantes[i].compteur = 0;
        tab_plante->plantes[i].etat_tige = 0;

        if (tab_x[i] > 0 && tab_x[i] < carte->largeur - 1) {
            carte->carte[tab_y[i]][tab_x[i]-1] = ']';
            carte->carte[tab_y[i]][tab_x[i]] = 'u';
            carte->carte[tab_y[i]][tab_x[i]+1] = '[';
        }
    }
}

void bougerPlante(Carte *carte, Tab_plante *tab_plante)
{
    if (tab_plante == NULL || tab_plante->plantes == NULL || tab_plante->count <= 0)
    {
        return;
    }

    static int pause_counter[10] = {0};
    static int etat_deplacement[10] = {0}; 
    static int montee_delai[10] = {0}; 
    static int descente_delai[10] = {0}; 

    for (int i = 0; i < tab_plante->count; i++)
    {
        int oldY = tab_plante->plantes[i].positionY;
        int oldX = tab_plante->plantes[i].positionX;

        carte->carte[oldY][oldX] = ' ';

        switch (etat_deplacement[i]) {
            case 0:
                for (int y = oldY + 1; y <= tab_plante->plantes[i].positionY_base; y++) {
                    carte->carte[y][oldX] = '|';
                }
                
                montee_delai[i]++;
                if (montee_delai[i] >= 3) {
                    montee_delai[i] = 0;
                    if (tab_plante->plantes[i].positionY > 0 && tab_plante->plantes[i].compteur < 2)
                    {
                        tab_plante->plantes[i].positionY--;
                        tab_plante->plantes[i].compteur++;
                        tab_plante->plantes[i].etat_tige = 1;
                        if (tab_plante->plantes[i].compteur == 2) {
                            etat_deplacement[i] = 1; 
                        }
                    } else {
                        etat_deplacement[i] = 1; 
                    }
                }
                break;

            case 1:
                for (int y = tab_plante->plantes[i].positionY + 1; y <= tab_plante->plantes[i].positionY_base; y++) {
                    carte->carte[y][oldX] = '|';
                }
                pause_counter[i]++;
                if (pause_counter[i] >= 25) {
                    pause_counter[i] = 0;
                    etat_deplacement[i] = 2; 
                }
                break;

            case 2:
                descente_delai[i]++;
                if (descente_delai[i] >= 3) {
                    descente_delai[i] = 0;
                    if (tab_plante->plantes[i].positionY < tab_plante->plantes[i].positionY_base && tab_plante->plantes[i].compteur > 0)
                    {
                        for (int y = oldY + 1; y <= tab_plante->plantes[i].positionY_base; y++) {
                            carte->carte[y][oldX] = '|';
                        }
                        tab_plante->plantes[i].positionY++;
                        tab_plante->plantes[i].compteur--;
                        if (tab_plante->plantes[i].positionY == tab_plante->plantes[i].positionY_base) {
                            tab_plante->plantes[i].etat_tige = 0;
                            etat_deplacement[i] = 3; 
                        }
                    } else {
                        etat_deplacement[i] = 3;
                    }
                } else {
                    for (int y = oldY + 1; y <= tab_plante->plantes[i].positionY_base; y++) {
                        carte->carte[y][oldX] = '|';
                    }
                }
                break;

            case 3:
                pause_counter[i]++;
                if (pause_counter[i] >= 15) {
                    pause_counter[i] = 0;
                    etat_deplacement[i] = 0; 
                }
                break;
        }

        int newY = tab_plante->plantes[i].positionY;
        int newX = tab_plante->plantes[i].positionX;

        if (newY >= 0 && newY < carte->hauteur && newX >= 0 && newX < carte->largeur)
        {
            carte->carte[newY][newX] = 'u';
            if (newX > 0 && newX < carte->largeur - 1) {
                carte->carte[tab_plante->plantes[i].positionY_base][newX-1] = ']';
                carte->carte[tab_plante->plantes[i].positionY_base][newX+1] = '[';
            }
            for (int y = newY + 1; y < tab_plante->plantes[i].positionY_base; y++) {
                carte->carte[y][newX] = '|';
            }
        }
        else
        {
            printf("Erreur: Plante %d hors limites!\n", i);
        }
    }
}

int ecraser_gumba(Carte *carte, Tab_gumba *tab_gumba, Personnage *perso)
{
    if (perso->en_chute || (perso->en_saut && perso->etape_saut > 4))
    {
        for (int i = 0; i < tab_gumba->count; i++)
        {
            if (perso->positionX == tab_gumba->gumbas[i].positionX &&
                perso->positionY + 1 == tab_gumba->gumbas[i].positionY)
            {
                carte->carte[tab_gumba->gumbas[i].positionY][tab_gumba->gumbas[i].positionX] = ' ';
                
                if (i < tab_gumba->count - 1)
                {
                    tab_gumba->gumbas[i] = tab_gumba->gumbas[tab_gumba->count - 1];
                }
                
                tab_gumba->count--;
                
                if (tab_gumba->count > 0)
                {
                    Gumba *temp = realloc(tab_gumba->gumbas, tab_gumba->count * sizeof(Gumba));
                    if (temp != NULL)
                    {
                        tab_gumba->gumbas = temp;
                    }
                }
                else
                {
                    free(tab_gumba->gumbas);
                    tab_gumba->gumbas = NULL;
                }
                
                perso->score += 5;
                
                return 1;
            }
        }
    }
    return 0;
}

int collision_avec_gumba(Tab_gumba *tab_gumba, Personnage *perso)
{
    for (int i = 0; i < tab_gumba->count; i++)
    {
        if (perso->positionY == tab_gumba->gumbas[i].positionY &&
            (perso->positionX == tab_gumba->gumbas[i].positionX ||
             perso->positionX + 1 == tab_gumba->gumbas[i].positionX ||
             perso->positionX - 1 == tab_gumba->gumbas[i].positionX))
        {
            return 1;
        }
    }
    return 0;
}

int collision_avec_plante(Tab_plante *tab_plante, Personnage *perso)
{
    for (int i = 0; i < tab_plante->count; i++)
    {
        if (perso->positionY == tab_plante->plantes[i].positionY &&
            perso->positionX == tab_plante->plantes[i].positionX)
        {
            return 1;
        }
    }
    return 0;
}

void gerer_collisions(Carte *carte, Personnage *perso, Tab_gumba *tab_gumba, Tab_plante *tab_plante)
{
    if (ecraser_gumba(carte, tab_gumba, perso))
    {
        return;
    }
    
    if (collision_avec_gumba(tab_gumba, perso))
    {
        perso->positionY = MORT_Y + 1;
        return;
    }
    
    if (collision_avec_plante(tab_plante, perso))
    {
        perso->positionY = MORT_Y + 1;
        return;
    }
}