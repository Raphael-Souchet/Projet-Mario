#include "projet.h"

void effacer_position(FILE *fichier, Personnage *perso, int largeur) {
    fseek(fichier, (perso->positionY) * largeur + perso->positionX, SEEK_SET);
    fputc(' ', fichier);
}

void mettre_position(FILE *fichier, Personnage *perso, int largeur) {
    fseek(fichier, (perso->positionY) * largeur + perso->positionX, SEEK_SET);
    fputc('M', fichier);
}

void cacherCurseur() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    info.dwSize = 1;
    SetConsoleCursorInfo(console, &info);
}

void gerer_saut(FILE *fichier, Personnage *perso, int largeur, int direction) {
    int hauteur_saut[8] = {-1, -1, -1, -1, 1, 1, 1, 1}; 
    
    for (int i = 0; i < 8; i++) {
        int nouv_x = perso->positionX + direction;
        int nouv_y = perso->positionY + hauteur_saut[i];
        
        fseek(fichier, nouv_y * largeur + nouv_x, SEEK_SET);
        char c = fgetc(fichier);
        if (c == 'c') {
            perso->score++;
            fseek(fichier, nouv_y * largeur + nouv_x, SEEK_SET);
            fputc(' ', fichier);
        }
        
        char caractere_destination;
        fseek(fichier, nouv_y * largeur + nouv_x, SEEK_SET);
        caractere_destination = fgetc(fichier);

        char caractere_lateral;
        fseek(fichier, perso->positionY * largeur + nouv_x, SEEK_SET);
        caractere_lateral = fgetc(fichier);
        
        if (nouv_x >= 0 && nouv_x < largeur && nouv_y >= 0 && 
            caractere_destination != 'w' && caractere_lateral != 'w' && perso->peut_monter) {
            effacer_position(fichier, perso, largeur);
            perso->positionX = nouv_x;
            perso->positionY = nouv_y;
            mettre_position(fichier, perso, largeur);
            
            system("cls");
            afficherPaysage(fichier, perso->positionX);
            printf("Score: %d | Nom: %s | Vies: %d\n", perso->score, perso->nom, perso->vie);
            Sleep(75); 
        }
    }
    
    perso->en_saut = 0;
}

void caracterePaysage(char caractereActuel) {
    switch (caractereActuel) {
        case 'w':
            printf("#");
            break;
        case 'c':
            printf("o");
            break;
        case 'M':
            printf("&");
            break;
        default:
            printf("%c", caractereActuel);
            break;
    }
}

void afficherPaysage(FILE *fichier, int positionJoueur) {
    char ligne[1024];

    int debutLecture = positionJoueur - 41;
    int finLecture = positionJoueur + 41;

    if (debutLecture < 0) {
        debutLecture = 0;
    }
    if (finLecture < 82) {
        finLecture = 82;
    }

    rewind(fichier);

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        int longueurLigne = strlen(ligne);

        if (ligne[longueurLigne - 1] == '\n') {
            ligne[--longueurLigne] = '\0';
        }

        for (int i = 0; i < longueurLigne; i++) {
            if (i >= debutLecture && i < finLecture) {
                caracterePaysage(ligne[i]);
            } 
        }
        printf("\n"); 
    }
}

void verifier_collision(FILE *fichier, Personnage* perso, int largeur) {
    char caractere_dessous;
    fseek(fichier, (perso->positionY + 1) * largeur + perso->positionX, SEEK_SET);
    caractere_dessous = fgetc(fichier);
    
    if (caractere_dessous == 'w') {
        perso->en_chute = 0;
    } else {
        perso->en_chute = 1;
    }

    char caractere_dessus;
    fseek(fichier, (perso->positionY - 1) * largeur + perso->positionX, SEEK_SET);
    caractere_dessus = fgetc(fichier);
    
    if (caractere_dessus == 'w') {
        perso->peut_monter = 0;
    } else {
        perso->peut_monter = 1;
    }

    char caractere_devant;
    fseek(fichier, (perso->positionY) * largeur + perso->positionX + 1, SEEK_SET);
    caractere_devant = fgetc(fichier);
    
    if (caractere_devant == 'w' || perso->positionX == largeur - 3) {
        perso->peut_avancer = 0;
    } else {
        perso->peut_avancer = 1;
    }

    char caractere_derriere;
    fseek(fichier, (perso->positionY) * largeur + perso->positionX - 1, SEEK_SET);
    caractere_derriere = fgetc(fichier);
    
    if (caractere_derriere == 'w' || perso->positionX == 0) {
        perso->peut_reculer = 0;
    } else {
        perso->peut_reculer = 1;
    }

    if (perso->positionX < 0) perso->positionX = 0;
    if (perso->positionX >= largeur) perso->positionX = largeur - 1;
    if (perso->positionY < 0) perso->positionY = 0;
}

void verifier_collision_gumba(FILE *fichier, Gumba* gumba) {
    char caractere_devant_dessous;
    fseek(fichier, (gumba->positionY + 1) * LARGEUR_MAP + gumba->positionX + 1, SEEK_SET);
    caractere_devant_dessous = fgetc(fichier);
    
    if (caractere_devant_dessous == ' ') {
        gumba->peut_tomber_devant = 1;
    } else {
        gumba->peut_tomber_devant = 0;
    }

    char caractere_derriere_dessous;
    fseek(fichier, (gumba->positionY - 1) * LARGEUR_MAP + gumba->positionX - 1, SEEK_SET);
    caractere_derriere_dessous = fgetc(fichier);
    
    if (caractere_devant_dessous == ' ') {
        gumba->peut_tomber_derriere = 1;
    } else {
        gumba->peut_tomber_derriere = 0;
    }

    char caractere_devant;
    fseek(fichier, (gumba->positionY) * LARGEUR_MAP + gumba->positionX + 1, SEEK_SET);
    caractere_devant = fgetc(fichier);
    
    if (caractere_devant == 'w' || caractere_devant == ']' || caractere_devant == 'Q' || gumba->positionX == LARGEUR_MAP - 3) {
        gumba->peut_avancer = 0;
    } else {
        gumba->peut_avancer = 1;
    }

    char caractere_derriere;
    fseek(fichier, (gumba->positionY) * LARGEUR_MAP + gumba->positionX - 1, SEEK_SET);
    caractere_derriere = fgetc(fichier);
    
    if (caractere_derriere == 'w' || caractere_derriere == '[' || caractere_devant == 'Q' || gumba->positionX == 0) {
        gumba->peut_reculer = 0;
    } else {
        gumba->peut_reculer = 1;
    }

    if (gumba->positionX < 0) gumba->positionX = 0;
    if (gumba->positionX >= LARGEUR_MAP) gumba->positionX = LARGEUR_MAP - 1;
    if (gumba->positionY < 0) gumba->positionY = 0;
}

void deplacer_joueur(FILE *fichier, Personnage* perso, int largeur) {
    if (perso->positionY >= MORT_Y) {
        return; 
    }

    int deplacement_x = 0;
    
    if (GetAsyncKeyState('D') & 0x8000) deplacement_x = 1;
    if (GetAsyncKeyState('Q') & 0x8000) deplacement_x = -1; 

    verifier_collision(fichier, perso, largeur);

    if (perso->en_chute) {
        int new_y = perso->positionY + 1;
        fseek(fichier, new_y * largeur + perso->positionX, SEEK_SET);
        char c = fgetc(fichier);
        if (c == 'c') {
            perso->score++;
            fseek(fichier, new_y * largeur + perso->positionX, SEEK_SET);
            fputc(' ', fichier);
        }
        effacer_position(fichier, perso, largeur);
        perso->positionY = new_y;
        mettre_position(fichier, perso, largeur);
        verifier_collision(fichier, perso, largeur);
    }

    if (deplacement_x != 0 && !perso->en_saut) {
        int new_x = perso->positionX + deplacement_x;
        fseek(fichier, perso->positionY * largeur + new_x, SEEK_SET);
        char c = fgetc(fichier);
        if (c == 'c') {
            perso->score++;
            fseek(fichier, perso->positionY * largeur + new_x, SEEK_SET);
            fputc(' ', fichier);
        }
        if ((deplacement_x > 0 && perso->peut_avancer && perso->positionX < largeur - 1) || 
            (deplacement_x < 0 && perso->peut_reculer && perso->positionX > 0)) {
            effacer_position(fichier, perso, largeur);
            perso->positionX = new_x;
            mettre_position(fichier, perso, largeur);
        }
    }

    if ((GetAsyncKeyState('Z') & 0x8000) && !perso->en_saut && !perso->en_chute) {
        perso->en_saut = 1;
        int direction = 0;
        
        if (deplacement_x > 0) direction = 1;
        else if (deplacement_x < 0) direction = -1;
        
        gerer_saut(fichier, perso, largeur, direction);
    }

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        while (_kbhit()) _getch();
        menuSauvegarde(perso, fichier);
    }
}

void jouer(const char *fichierTemp, Personnage* perso) {    
    FILE *fichier = fopen(fichierTemp, "r+");
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", fichierTemp);
        Sleep(1500);
        return;
    }
    
    int largeur = LARGEUR_MAP;

    fseek(fichier, (perso->positionY) * largeur + perso->positionX, SEEK_SET);
    fputc('M', fichier);
    fflush(fichier);

    Tab_gumba tab_gumba = {NULL, 0};
    initialiser_gumbas(fichier, &tab_gumba);

    cacherCurseur();

    while (1) {
        Sleep(50);
        system("cls");
        
        rewind(fichier);
        bouger_gumba(fichier, &tab_gumba);
        afficherPaysage(fichier, perso->positionX);
        printf("Score: %d | Nom: %s | Vies: %d\n", perso->score, perso->nom, perso->vie);
        
        deplacer_joueur(fichier, perso, largeur);
        fflush(fichier);

        if (perso->positionY >= MORT_Y) {
            perso->vie--;
            fclose(fichier);
            menu_mort(perso, fichierTemp);
            return;
        }
    }
    free(tab_gumba.gumbas);
    tab_gumba.gumbas = NULL;
}