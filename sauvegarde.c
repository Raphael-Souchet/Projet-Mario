#include "projet.h"

char* creerNomFichierTemp(const char* nomJoueur) {
    char* fichierTemp = malloc(strlen(nomJoueur) + 9); 
    if (fichierTemp == NULL) {
        printf("Erreur d'allocation mémoire\n");
        exit(1);
    }
    sprintf(fichierTemp, "temp_%s.txt", nomJoueur);
    return fichierTemp;
}

int copierFichier(const char *source, const char *destination) {
    FILE *src = fopen(source, "r");
    if (src == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier source %s\n", source);
        return 0;
    }
    
    FILE *dest = fopen(destination, "w");
    if (dest == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier destination %s\n", destination);
        fclose(src);
        return 0;
    }

    char car;
    while ((car = fgetc(src)) != EOF) {
        fputc(car, dest);
    }

    fclose(src);
    fclose(dest);
    return 1;
}

void sauvegarderPartie(Personnage *perso, FILE *fichier) {
    fclose(fichier);
    
    FILE *sauvegarde = fopen("sauvegarde.txt", "r");
    char buffer[2048] = {0};
    if (sauvegarde) {
        fread(buffer, 1, sizeof(buffer)-1, sauvegarde);
        fclose(sauvegarde);
    }
    
    sauvegarde = fopen("sauvegarde.txt", "w");
    if (sauvegarde == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier de sauvegarde\n");
        Sleep(1500);
        return;
    }
    
    char *ptr = buffer;
    char *debut;
    while ((debut = strstr(ptr, "===== Sauvegarde"))) {
        char *fin = strstr(debut, "\n\n") + 2;
        char *nom_pos = strstr(debut, "Nom:") + 4;
        char nom[100];
        sscanf(nom_pos, "%99[^\n]", nom);
        
        if (strcmp(nom, perso->nom) != 0) {
            fwrite(fin, 1, fin - debut, sauvegarde);
        }
        ptr = fin;
    }
    
    time_t now;
    time(&now);
    char *date = ctime(&now);  
    date[strcspn(date, "\n")] = '\0';
    
    fprintf(sauvegarde, "===== Sauvegarde %s =====\n", date);
    fprintf(sauvegarde, "Nom:%s\n", perso->nom);
    fprintf(sauvegarde, "PositionX:%d\n", perso->positionX);
    fprintf(sauvegarde, "PositionY:%d\n", perso->positionY);
    fprintf(sauvegarde, "Score:%d\n", perso->score);
    fprintf(sauvegarde, "Vie:%d\n\n", perso->vie);
    fclose(sauvegarde);
    
    printf("Partie sauvegardee avec succes!\n");
    Sleep(1000);
    
    menuPrincipal("Mario.txt");
}

int chargerPartie(Personnage *perso) {
    FILE *fichier = fopen("sauvegarde.txt", "r");
    if (fichier == NULL) {
        printf("Aucune sauvegarde trouvee !\n");
        Sleep(1000);
        return 0;
    }

    Sauvegarde sauvegardes[100];
    int nbSauvegardes = 0;
    char ligne[100], nom_temp[100], date_temp[100];
    int score_temp = 0;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strstr(ligne, "===== Sauvegarde") == ligne) {
            char *debut = strchr(ligne, ' ') + 1;
            char *fin = strstr(debut, " =====");
            if (fin) *fin = '\0';
            strcpy(date_temp, debut);
        }
        else if (sscanf(ligne, "Nom:%s", nom_temp) == 1) {
        }
        else if (sscanf(ligne, "Score:%d", &score_temp) == 1) {
            strcpy(sauvegardes[nbSauvegardes].nom, nom_temp);
            sauvegardes[nbSauvegardes].score = score_temp;
            strcpy(sauvegardes[nbSauvegardes].date, date_temp);
            nbSauvegardes++;
        }
        else if (sscanf(ligne, "Vie:%d", &sauvegardes[nbSauvegardes - 1].vie) == 1) {
        }
    }

    fclose(fichier);

    if (nbSauvegardes == 0) {
        printf("Aucune sauvegarde disponible.\n");
        Sleep(1000);
        return 0;
    }

    printf("+------+--------------------+--------+------+-----------------------------+\n");
    printf("| #    | Nom                | Score  | Vie  | Date                        |\n");
    printf("+------+--------------------+--------+------+-----------------------------+\n");
    for (int i = 0; i < nbSauvegardes; i++) {
        printf("| %3d  | %-18s | %6d | %4d | %-27s |\n", 
               i + 1, 
               sauvegardes[i].nom, 
               sauvegardes[i].score, 
               sauvegardes[i].vie, 
               sauvegardes[i].date);
    }
    printf("+------+--------------------+--------+------+-----------------------------+\n");

    int choix;
    printf("Entrez le numero de la sauvegarde a charger: ");
    scanf("%d", &choix);
    while (choix < 1 || choix > nbSauvegardes) {
        printf("Numero invalide. Entrez un numero valide: ");
        scanf("%d", &choix);
    }

    fichier = fopen("sauvegarde.txt", "r");
    int currentSave = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strstr(ligne, "===== Sauvegarde") == ligne) {
            currentSave++;
        }
        if (currentSave == choix) {
            if (sscanf(ligne, "Nom:%s", perso->nom) == 1);
            else if (sscanf(ligne, "PositionX:%d", &perso->positionX) == 1); 
            else if (sscanf(ligne, "PositionY:%d", &perso->positionY) == 1); 
            else if (sscanf(ligne, "Score:%d", &perso->score) == 1); 
            else if (sscanf(ligne, "Vie:%d", &perso->vie) == 1);
        }
    }
    fclose(fichier);
    printf("Partie chargee avec succes !\n");
    Sleep(1500);
    return 1;
}

void resetScores() {
    printf("Voulez-vous vraiment supprimer toutes les sauvegardes ? (o/n) : ");
    char choix;
    scanf(" %c", &choix);
    
    if (choix == 'o' || choix == 'O') {
        FILE *fichier = fopen("sauvegarde.txt", "w");
        if (fichier != NULL) {
            fclose(fichier);
            printf("Les scores ont ete reinitialises avec succes !\n");
        } else {
            printf("Erreur: Impossible d'ouvrir le fichier de sauvegarde\n");
        }
        system("del temp_*.txt");
        printf("Les fichiers temporaires ont ete supprimes avec succes !\n");
        Sleep(1500);
    }
}

void afficherScores() {
    FILE *fichier = fopen("sauvegarde.txt", "r");
    if (fichier == NULL) {
        printf("Aucune sauvegarde trouvee !\n");
        Sleep(1000);
        return;
    }

    Sauvegarde sauvegardes[100]; 
    int nbSauvegardes = 0;
    char ligne[100];
    char nom_temp[100];
    int score_temp = 0;
    char date_temp[100];

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strstr(ligne, "===== Sauvegarde") == ligne) {
            char *start = ligne + 16;
            char *end = strstr(start, " =====");
            if (end) {
                *end = '\0';
                strcpy(date_temp, start);
            }
        }
        else if (sscanf(ligne, "Nom:%s", nom_temp) == 1) {
        }
        else if (sscanf(ligne, "Score:%d", &score_temp) == 1) {
            strcpy(sauvegardes[nbSauvegardes].nom, nom_temp);
            sauvegardes[nbSauvegardes].score = score_temp;
            strcpy(sauvegardes[nbSauvegardes].date, date_temp);
            nbSauvegardes++;
        }
        else if (sscanf(ligne, "Vie:%d", &sauvegardes[nbSauvegardes - 1].vie) == 1) {
        }
    }
    
    fclose(fichier);

    for (int i = 0; i < nbSauvegardes - 1; i++) {
        for (int j = 0; j < nbSauvegardes - i - 1; j++) {
            if (sauvegardes[j].score < sauvegardes[j + 1].score) {
                Sauvegarde temp = sauvegardes[j];
                sauvegardes[j] = sauvegardes[j + 1];
                sauvegardes[j + 1] = temp;
            }
        }
    }

    printf("+------+--------------------+--------+------+-----------------------------+\n");
    printf("| #    | Nom                | Score  | Vie  | Date                        |\n");
    printf("+------+--------------------+--------+------+-----------------------------+\n");

    for (int i = 0; i < nbSauvegardes && i < 10; i++) {
        printf("| %3d  | %-18s | %6d | %4d | %-27s |\n", 
               i + 1, 
               sauvegardes[i].nom, 
               sauvegardes[i].score, 
               sauvegardes[i].vie, 
               sauvegardes[i].date);
    }
    printf("+------+--------------------+--------+------+-----------------------------+\n");

    printf("\nAppuyez sur une touche pour continuer...");
    _getch();
}