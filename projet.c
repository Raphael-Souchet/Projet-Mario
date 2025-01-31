#include "projet.h"

void effacer_position(FILE *fichier, Personnage *perso, int largeur) {
    fseek(fichier, (perso->positionY) * largeur + perso->positionX, SEEK_SET);
    fputc(' ', fichier);
}

void mettre_position(FILE *fichier, Personnage *perso, int largeur) {
    fseek(fichier, (perso->positionY) * largeur + perso->positionX, SEEK_SET);
    fputc('M', fichier);
}

void gerer_saut(FILE *fichier, Personnage *perso, int largeur, int direction) {
    int positions_saut[4][2];
    
    if (direction == 0) {  
        int saut_vertical[4][2] = {{0, -1}, {0, -1}, {0, 1}, {0, 1}};
        memcpy(positions_saut, saut_vertical, sizeof(positions_saut));
    }
    else if (direction == 1) { 
        int saut_droite[4][2] = {{1, -1}, {1, -1}, {1, 1}, {1, 1}};
        memcpy(positions_saut, saut_droite, sizeof(positions_saut));
    }
    else { 
        int saut_gauche[4][2] = {{-1, -1}, {-1, -1}, {-1, 1}, {-1, 1}};
        memcpy(positions_saut, saut_gauche, sizeof(positions_saut));
    }
    
    for (int i = 0; i < 4; i++) {
        int ancien_x = perso->positionX;
        int ancien_y = perso->positionY;
        
        int nouveau_x = perso->positionX + positions_saut[i][0];
        int nouveau_y = perso->positionY + positions_saut[i][1];
        
        if (nouveau_x >= 0 && nouveau_x < largeur && nouveau_y >= 0) {
            effacer_position(fichier, perso, largeur);
            
            perso->positionX = nouveau_x;
            perso->positionY = nouveau_y;
            
            mettre_position(fichier, perso, largeur);
            
            system("cls");
            afficherPaysage(fichier, perso->positionX);
            Sleep(100);
        } else {
            perso->positionX = ancien_x;
            perso->positionY = ancien_y;
        }
    }
    
    perso->en_saut = 0;
}

void copierFichier(const char *source, const char *destination) {
    FILE *src = fopen(source, "r");
    FILE *dest = fopen(destination, "w");

    char car;
    while ((car = fgetc(src)) != EOF) {
        fputc(car, dest);
    }

    fclose(src);
    fclose(dest);
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

    int debutLecture = positionJoueur - 21;
    int finLecture = positionJoueur + 21;

    if (debutLecture < 0) {
        debutLecture = 0;
    }
    if (finLecture < 42) {
        finLecture = 42;
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
            } else {
                continue;
            }
        }
        printf("\n"); 
    }
}

void deplacer_joueur(FILE *fichier, Personnage* perso, int largeur) {
    if (_kbhit()) {
        int direction = 0;  
        char touche = _getch();
        while (_kbhit()) {
            _getch();
        }
        
        if (_kbhit()) {
            char deuxieme_touche = _getch();
            
            if ((touche == 'z' && (deuxieme_touche == 'd' || deuxieme_touche == 'q')) ||
                ((touche == 'd' || touche == 'q') && deuxieme_touche == 'z')) {
                if (touche == 'd' || deuxieme_touche == 'd') {
                    direction = 1;  
                } else {
                    direction = -1; 
                }
                
                if (!perso->en_saut) {
                    perso->en_saut = 1;
                    gerer_saut(fichier, perso, largeur, direction);
                    return;
                }
            }
        }
        
        if (!perso->en_saut) {
            int nouvelle_posx = perso->positionX;
            int nouvelle_posy = perso->positionY;

            switch (touche) {
                case 'q': 
                    if (perso->positionX > 0) nouvelle_posx--;
                    break;
                case 'd': 
                    if (perso->positionX < largeur - 1) nouvelle_posx++;
                    break;
                case 'z': 
                    perso->en_saut = 1;
                    gerer_saut(fichier, perso, largeur, 0);  
                    return;
                case 'e': 
                    exit(0);
                case 's':
                case 'S':
                    menuSauvegarde(perso, "temp.txt");
                    return;
            }

            if (nouvelle_posx != perso->positionX || nouvelle_posy != perso->positionY) {
                effacer_position(fichier, perso, largeur);
                perso->positionX = nouvelle_posx;
                perso->positionY = nouvelle_posy;
                mettre_position(fichier, perso, largeur);
            }
        }

        rewind(fichier);
    }
}

void menuPrincipal(const char *fichierTemp) {
    Personnage perso = {21, 5, "Mario", 0, 0};
    int choix;
    
    system("cls");
    printf("+---------------------------------------------+\n");
    printf("|     Bienvenue sur le meilleur jeu Mario     |\n");
    printf("|                                             |\n");  
    printf("|                   - Menu -                  |\n");  
    printf("|                                             |\n");  
    printf("|                1. Nouvelle Partie           |\n");  
    printf("|                2. Charger Partie            |\n");  
    printf("|                3. Score                     |\n");  
    printf("|                4. Quitter                   |\n"); 
    printf("+---------------------------------------------+\n");
    printf("Choisissez une option : ");
    scanf("%d", &choix);

    if (choix < 1 || choix > 4) {
        printf("Choix invalide, veuillez reessayer\n");
        Sleep(1000);
        menuPrincipal(fichierTemp);
        return;
    }
    
    system("cls");
    
    switch(choix) {
        case 1:
            printf("Entrez votre nom: ");
            scanf("%s", perso.nom);
            jouer(fichierTemp, &perso);
            break;
        case 2:
            if (chargerPartie(&perso)) {
                jouer(fichierTemp, &perso);
            } else {
                menuPrincipal(fichierTemp);
            }
            break;
        case 3:
            afficherScores();
            menuPrincipal(fichierTemp);
            break;
        case 4:
            exit(0);
    }
}

void sauvegarderPartie(Personnage *perso) {
    FILE *fichier = fopen(FICHIER_SAUVEGARDE, "a");
    if (fichier == NULL) {
        printf("Erreur lors de la creation du fichier de sauvegarde!\n");
        return;
    }

    time_t now;
    time(&now);
    
    fprintf(fichier, "===== Sauvegarde %s =====\n", ctime(&now));
    fprintf(fichier, "Nom:%s\n", perso->nom);
    fprintf(fichier, "PositionX:%d\n", perso->positionX);
    fprintf(fichier, "PositionY:%d\n", perso->positionY);
    fprintf(fichier, "EnSaut:%d\n", perso->en_saut);
    fprintf(fichier, "Score:%d\n\n", perso->score);  

    fclose(fichier);
    printf("Partie sauvegardee avec succes!\n");
    Sleep(1000);
}

int chargerPartie(Personnage *perso) {
    FILE *fichier = fopen(FICHIER_SAUVEGARDE, "r");
    if (fichier == NULL) {
        printf("Aucune sauvegarde trouvee!\n");
        Sleep(1000);
        return 0;
    }

    char ligne[100];
    char valeur[100];

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (sscanf(ligne, "Nom:%s", valeur) == 1) {
            strcpy(perso->nom, valeur);
        }
        else if (sscanf(ligne, "PositionX:%d", &perso->positionX) == 1) {}
        else if (sscanf(ligne, "PositionY:%d", &perso->positionY) == 1) {}
        else if (sscanf(ligne, "EnSaut:%d", &perso->en_saut) == 1) {}
        else if (sscanf(ligne, "Score:%d", &perso->score) == 1) {}
    }

    fclose(fichier);
    printf("Partie chargee avec succes!\n");
    printf("Bienvenue %s! Score: %d\n", perso->nom, perso->score);
    Sleep(1000);
    return 1;
}

void menuSauvegarde(Personnage *perso, const char *fichierTemp) {
    system("cls");
    printf("+---------------------------------------------+\n");
    printf("|              Menu Sauvegarde                |\n");
    printf("|                                             |\n");
    printf("|           1. Sauvegarder la partie         |\n");
    printf("|           2. Charger une partie            |\n");
    printf("|           3. Retour au jeu                 |\n");
    printf("|           4. Menu Principal                |\n");
    printf("+---------------------------------------------+\n");
    printf("Votre choix: ");
    
    int choix;
    scanf("%d", &choix);

    switch(choix) {
        case 1:
            sauvegarderPartie(perso);
            break;
        case 2:
            if (chargerPartie(perso)) {
                jouer(fichierTemp, perso);
            }
            break;
        case 3:
            break;
        case 4:
            menuPrincipal(fichierTemp);
            break;
        default:
            printf("Choix invalide!\n");
            Sleep(1000);
    }
}

void afficherScores() {
    FILE *fichier = fopen(FICHIER_SAUVEGARDE, "r");
    if (fichier == NULL) {
        printf("Aucune sauvegarde trouvee!\n");
        Sleep(1000);
        return;
    }

    printf("+---------------------------------------------+\n");
    printf("|                  Scores                     |\n");
    printf("+---------------------------------------------+\n");

    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        printf("%s", ligne);
    }

    fclose(fichier);
    
    printf("\nAppuyez sur une touche pour continuer...");
    _getch();
}

void jouer(const char *fichierTemp, Personnage* perso) {
    FILE *fichier = fopen(fichierTemp, "r+");

    fseek(fichier, (perso->positionY) * 100 + perso->positionX, SEEK_SET);
    fputc('M', fichier);

    int largeur = 100; 
    while (1) {
        system("cls"); 
        afficherPaysage(fichier, perso->positionX);
        printf("\nAppuyez sur 'S' pour ouvrir le menu de sauvegarde\n");
        
        deplacer_joueur(fichier, perso, largeur);
        Sleep(200);
    }
    
    fclose(fichier);
}