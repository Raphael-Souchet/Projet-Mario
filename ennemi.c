#include "projet.h"

void initialiser_gumbas(FILE* fichier, Tab_gumba* tab_gumba) {
    if (tab_gumba == NULL) {
        printf("Erreur: tab_gumba non valide\n");
        return;
    }
    
    tab_gumba->count = 4;
    tab_gumba->gumbas = malloc(tab_gumba->count * sizeof(Gumba));
    
    if (tab_gumba->gumbas == NULL) {
        printf("Erreur: allocation de mémoire pour les gumbas échouée\n");
        tab_gumba->count = 0;
        return;
    }
    
    int tab_x[4] = {93, 98, 126, 143};
    int tab_y[4] = {13, 8, 7, 7};
    
    for (int i = 0; i < tab_gumba->count; i++) {
        tab_gumba->gumbas[i].positionX = tab_x[i];
        tab_gumba->gumbas[i].positionY = tab_y[i];
        tab_gumba->gumbas[i].peut_avancer = 1;
        tab_gumba->gumbas[i].peut_reculer = 0;
        tab_gumba->gumbas[i].peut_tomber_devant = 0;
        tab_gumba->gumbas[i].peut_tomber_derriere = 0;
        tab_gumba->gumbas[i].dernier_deplacement = 1;
        tab_gumba->gumbas[i].compteur = 0;
        
        fseek(fichier, (tab_gumba->gumbas[i].positionY) * LARGEUR_MAP + tab_gumba->gumbas[i].positionX, SEEK_SET);
        fputc('Q', fichier);
    }
}

void bouger_gumba(FILE* fichier, Tab_gumba* tab_gumba) {
    if (tab_gumba == NULL || tab_gumba->gumbas == NULL || tab_gumba->count <= 0) {
        return;
    }
    
    for (int i = 0; i < tab_gumba->count; i++) {
        fseek(fichier, (tab_gumba->gumbas[i].positionY) * LARGEUR_MAP + tab_gumba->gumbas[i].positionX, SEEK_SET);
        fputc(' ', fichier);
        verifier_collision_gumba(fichier, &tab_gumba->gumbas[i]);
        
        if (tab_gumba->gumbas[i].dernier_deplacement) {
            if (!tab_gumba->gumbas[i].peut_tomber_devant && tab_gumba->gumbas[i].peut_avancer) {
                tab_gumba->gumbas[i].positionX++;
            }
            else {
                tab_gumba->gumbas[i].positionX--;
                tab_gumba->gumbas[i].dernier_deplacement = 0;
            }
        }
        else {
            if (!tab_gumba->gumbas[i].peut_tomber_derriere && tab_gumba->gumbas[i].peut_reculer) {
                tab_gumba->gumbas[i].positionX--;
            }
            else {
                tab_gumba->gumbas[i].positionX++;
                tab_gumba->gumbas[i].dernier_deplacement = 1;
            }
        }
        
        fseek(fichier, (tab_gumba->gumbas[i].positionY) * LARGEUR_MAP + tab_gumba->gumbas[i].positionX, SEEK_SET);
        fputc('Q', fichier);
    }
}