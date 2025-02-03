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
    int hauteur_saut[8] = {-1, -1, -1, -1, 1, 1, 1, 1}; 
    
    for (int i = 0; i < 8; i++) {
        int nouv_x = perso->positionX + direction;
        int nouv_y = perso->positionY + hauteur_saut[i];
        
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
            Sleep(75); 
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
    
    if (caractere_devant == 'w') {
        perso->peut_avancer = 0;
    } else {
        perso->peut_avancer = 1;
    }

    char caractere_derriere;
    fseek(fichier, (perso->positionY) * largeur + perso->positionX - 1, SEEK_SET);
    caractere_derriere = fgetc(fichier);
    
    if (caractere_derriere == 'w') {
        perso->peut_reculer = 0;
    } else {
        perso->peut_reculer = 1;
    }
}

void deplacer_joueur(FILE *fichier, Personnage* perso, int largeur) {
    int deplacement_x = 0;
    
    if (GetAsyncKeyState('D') & 0x8000) deplacement_x = 1;
    if (GetAsyncKeyState('Q') & 0x8000) deplacement_x = -1; 

    verifier_collision(fichier, perso, largeur);

    if (perso->en_chute) {
        effacer_position(fichier, perso, largeur);
        perso->positionY++;
        mettre_position(fichier, perso, largeur);
    }

    if (deplacement_x != 0 && !perso->en_saut) {
        if ((deplacement_x > 0 && perso->peut_avancer) || 
            (deplacement_x < 0 && perso->peut_reculer)) {
            effacer_position(fichier, perso, largeur);
            perso->positionX += deplacement_x;
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

    if (GetAsyncKeyState('S') & 0x8000) {
        menuSauvegarde(perso, "temp.txt");
    }
}

void menuPrincipal(const char *fichierTemp) {
    Personnage perso = {21, 5, "Mario"};
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
    FILE *fichier = fopen("sauvegarde.txt", "a");
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
    FILE *fichier = fopen("sauvegarde.txt", "r");
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
    printf("|           1. Sauvegarder la partie          |\n");
    printf("|           2. Charger une partie             |\n");
    printf("|           3. Retour au jeu                  |\n");
    printf("|           4. Menu Principal                 |\n");
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
    FILE *fichier = fopen("sauvegarde.txt", "r");
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
    int largeur = 100;

    fseek(fichier, (perso->positionY) * 100 + perso->positionX, SEEK_SET);
    fputc('M', fichier);

    while (1) {
        Sleep(50);
        
        if (_kbhit()) {
            char touche = _getch();
            if (touche == 'e') exit(0);
        }

        system("cls");
        afficherPaysage(fichier, perso->positionX);
        deplacer_joueur(fichier, perso, largeur);
        fflush(fichier);
    }
    
    fclose(fichier);
}