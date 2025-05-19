#include "projet.h"

void initialiser_gumbas(Carte *carte, Tab_gumba *tab_gumba, int niveau)
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
            printf("Erreur: allocation mémoire gumbas échouée\n");
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

    const int *tab_x = NULL, *tab_y = NULL;
    int count = 0;

    switch (niveau)
    {
        case 0: {
            static int x0[] = {74, 80, 120};
            static int y0[] = {14, 9, 8};
            tab_x = x0; tab_y = y0; count = 3;
            break;
        }
        case 1: {
            static int x1[] = {56, 72, 92};
            static int y1[] = {20, 8, 17};
            tab_x = x1; tab_y = y1; count = 3;
            break;
        }
        case 2: {
            static int x2[] = {30, 90, 95};
            static int y2[] = {12, 11, 10};
            tab_x = x2; tab_y = y2; count = 3;
            break;
        }
        case 3: {
            static int x3[] = {40, 42};
            static int y3[] = {14, 14};
            tab_x = x3; tab_y = y3; count = 2;
            break;
        }
        default:
            return;
    }

    tab_gumba->count = count;
    tab_gumba->gumbas = malloc(count * sizeof(Gumba));
    if (tab_gumba->gumbas == NULL)
    {
        printf("Erreur: allocation mémoire gumbas échouée\n");
        tab_gumba->count = 0;
        return;
    }

    for (int i = 0; i < count; i++)
    {
        tab_gumba->gumbas[i].positionX = tab_x[i];
        tab_gumba->gumbas[i].positionY = tab_y[i];
        tab_gumba->gumbas[i].peut_avancer = 1;
        tab_gumba->gumbas[i].peut_reculer = 1;
        tab_gumba->gumbas[i].peut_tomber_devant = 0;
        tab_gumba->gumbas[i].peut_tomber_derriere = 0;
        tab_gumba->gumbas[i].dernier_deplacement = 1;

        carte->carte[tab_y[i]][tab_x[i]] = 'Q';
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

void initialiserPlante(Carte *carte, Tab_plante *tab_plante, int niveau)
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

    const int *tab_x = NULL, *tab_y = NULL;
    int count = 0;

    switch (niveau)
    {
        case 0: {
            static int x0[] = {88, 94};
            static int y0[] = {14, 9};
            tab_x = x0; tab_y = y0; count = 2;
            break;
        }
        case 1: {
            static int x1[] = {50, 90};
            static int y1[] = {10, 10};
            tab_x = x1; tab_y = y1; count = 2;
            break;
        }
        case 2: {
            static int x2[] = {100, 105};
            static int y2[] = {10, 9};
            tab_x = x2; tab_y = y2; count = 2;
            break;
        }
        case 3: {
            static int x3[] = {60};
            static int y3[] = {13};
            tab_x = x3; tab_y = y3; count = 1;
            break;
        }
        default:
            return;
    }

    tab_plante->count = count;
    tab_plante->plantes = malloc(count * sizeof(Plante));

    if (tab_plante->plantes == NULL)
    {
        printf("Erreur: allocation de mémoire pour les plantes échouée\n");
        tab_plante->count = 0;
        return;
    }

    for (int i = 0; i < count; i++)
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

        if (tab_x[i] > 0 && tab_x[i] < carte->largeur - 1)
        {
            carte->carte[tab_y[i]][tab_x[i]] = ']';
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
        int baseX = tab_plante->plantes[i].positionX;
        int baseY = tab_plante->plantes[i].positionY_base;
        
        int trouY = baseY - 4; 
        
        for (int y = trouY + 1; y < baseY; y++) {
            carte->carte[y][baseX] = ' ';
        }
        
        int currentY = tab_plante->plantes[i].positionY;

        carte->carte[baseY][baseX] = ']';
        
        switch (etat_deplacement[i]) {
            case 0:
                montee_delai[i]++;
                if (montee_delai[i] >= 3) {
                    montee_delai[i] = 0;
                    if (currentY > baseY - 3 && tab_plante->plantes[i].compteur < 2)
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
                    if (currentY < baseY && tab_plante->plantes[i].compteur > 0)
                    {
                        tab_plante->plantes[i].positionY++;
                        tab_plante->plantes[i].compteur--;
                        if (tab_plante->plantes[i].positionY == baseY) {
                            tab_plante->plantes[i].etat_tige = 0;
                            etat_deplacement[i] = 3; 
                        }
                    } else {
                        etat_deplacement[i] = 3;
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

        currentY = tab_plante->plantes[i].positionY;
        
        if (currentY < baseY) {
            if (tab_plante->plantes[i].etat_tige == 1) {
                carte->carte[currentY][baseX] = 'u';
                
                for (int y = currentY + 1; y < baseY; y++) {
                    carte->carte[y][baseX] = '|';
                }
            }
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
                
                declencher_rebond(perso);
                
                return 1;
            }
        }
    }
    return 0;
}

void declencher_rebond(Personnage *perso)
{
    perso->en_saut = 1;
    perso->etape_saut = 1;
    perso->en_chute = 0;
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
    if (!tab_plante || !tab_plante->plantes)
        return 0;
        
    for (int i = 0; i < tab_plante->count; i++)
    {
        Plante *plante = &tab_plante->plantes[i];
        
        if (plante->positionX == perso->positionX && 
            plante->positionY == perso->positionY)
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