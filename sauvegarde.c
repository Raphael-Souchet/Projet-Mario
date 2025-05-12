#include "projet.h"

char *creerNomFichierTemp(const char *nomJoueur)
{
    char *fichierTemp = malloc(strlen(nomJoueur) + 9);
    if (fichierTemp == NULL)
    {
        printf("Erreur d'allocation mémoire\n");
        exit(1);
    }
    sprintf(fichierTemp, "temp_%s.txt", nomJoueur);
    return fichierTemp;
}

int copierFichier(const char *source, const char *destination)
{
    FILE *src = fopen(source, "r");
    if (src == NULL)
    {
        printf("Erreur: Impossible d'ouvrir le fichier source %s\n", source);
        return 0;
    }

    FILE *dest = fopen(destination, "w");
    if (dest == NULL)
    {
        printf("Erreur: Impossible d'ouvrir le fichier destination %s\n", destination);
        fclose(src);
        return 0;
    }

    char car;
    while ((car = fgetc(src)) != EOF)
    {
        fputc(car, dest);
    }

    fclose(src);
    fclose(dest);
    return 1;
}

void sauvegarderPartie(Personnage *perso, Carte *carte, const char *fichierTemp)
{
    sauvegarderCarteVersFichier(carte, fichierTemp);

    typedef struct
    {
        char nom[100];
        int positionX;
        int positionY;
        int score;
        int vie;
        char date[100];
    } SaveData;

    SaveData sauvegardes[100];
    int nbSauvegardes = 0;
    int joueurExiste = -1;

    FILE *fichier = fopen("sauvegarde.txt", "r");
    if (fichier != NULL)
    {
        char ligne[1024];
        int currentSave = -1;

        while (fgets(ligne, sizeof(ligne), fichier))
        {
            ligne[strcspn(ligne, "\n")] = 0;

            if (strncmp(ligne, "===== Sauvegarde", 16) == 0)
            {
                currentSave = nbSauvegardes++;
                char *dateStart = ligne + 16;
                strcpy(sauvegardes[currentSave].date, dateStart);
                char *endMarker = strstr(sauvegardes[currentSave].date, " =====");
                if (endMarker)
                    *endMarker = '\0';
            }
            else if (strncmp(ligne, "Nom:", 4) == 0)
            {
                strcpy(sauvegardes[currentSave].nom, ligne + 4);
                if (strcmp(sauvegardes[currentSave].nom, perso->nom) == 0)
                {
                    joueurExiste = currentSave;
                }
            }
            else if (strncmp(ligne, "PositionX:", 10) == 0)
            {
                sauvegardes[currentSave].positionX = atoi(ligne + 10);
            }
            else if (strncmp(ligne, "PositionY:", 10) == 0)
            {
                sauvegardes[currentSave].positionY = atoi(ligne + 10);
            }
            else if (strncmp(ligne, "Score:", 6) == 0)
            {
                sauvegardes[currentSave].score = atoi(ligne + 6);
            }
            else if (strncmp(ligne, "Vie:", 4) == 0)
            {
                sauvegardes[currentSave].vie = atoi(ligne + 4);
            }
        }
        fclose(fichier);
    }

    time_t now;
    time(&now);
    char *date = ctime(&now);
    date[strcspn(date, "\n")] = '\0';

    if (joueurExiste >= 0)
    {
        sauvegardes[joueurExiste].positionX = perso->positionX;
        sauvegardes[joueurExiste].positionY = perso->positionY;
        sauvegardes[joueurExiste].score = perso->score;
        sauvegardes[joueurExiste].vie = perso->vie;
        strcpy(sauvegardes[joueurExiste].date, date);
    }
    else
    {
        strcpy(sauvegardes[nbSauvegardes].nom, perso->nom);
        sauvegardes[nbSauvegardes].positionX = perso->positionX;
        sauvegardes[nbSauvegardes].positionY = perso->positionY;
        sauvegardes[nbSauvegardes].score = perso->score;
        sauvegardes[nbSauvegardes].vie = perso->vie;
        strcpy(sauvegardes[nbSauvegardes].date, date);
        nbSauvegardes++;
    }

    fichier = fopen("sauvegarde.txt", "w");
    if (fichier == NULL)
    {
        printf("Erreur: Impossible d'ouvrir le fichier de sauvegarde\n");
        Sleep(1500);
        return;
    }

    for (int i = 0; i < nbSauvegardes; i++)
    {
        fprintf(fichier, "===== Sauvegarde %s =====\n", sauvegardes[i].date);
        fprintf(fichier, "Nom:%s\n", sauvegardes[i].nom);
        fprintf(fichier, "PositionX:%d\n", sauvegardes[i].positionX);
        fprintf(fichier, "PositionY:%d\n", sauvegardes[i].positionY);
        fprintf(fichier, "Score:%d\n", sauvegardes[i].score);
        fprintf(fichier, "Vie:%d\n\n", sauvegardes[i].vie);
    }

    fclose(fichier);

    printf("Partie sauvegardee avec succes!\n");
    Sleep(1000);

    libererCarte(carte);
    menuPrincipal("Mario.txt");
}

int chargerPartie(Personnage *perso)
{
    FILE *fichier = fopen("sauvegarde.txt", "r");
    if (fichier == NULL)
    {
        printf("Aucune sauvegarde trouvee !\n");
        Sleep(1000);
        return 0;
    }

    Sauvegarde sauvegardes[100];
    int nbSauvegardes = 0;
    char ligne[256];
    int dans_sauvegarde = 0;
    int sauvegarde_courante = -1;

    while (fgets(ligne, sizeof(ligne), fichier))
    {
        ligne[strcspn(ligne, "\n")] = 0;

        if (strncmp(ligne, "===== Sauvegarde", 16) == 0)
        {
            sauvegarde_courante = nbSauvegardes;
            nbSauvegardes++;
            dans_sauvegarde = 1;

            char *dateStart = ligne + 16;
            char *dateEnd = strstr(dateStart, " =====");
            if (dateEnd)
                *dateEnd = '\0';
            strcpy(sauvegardes[sauvegarde_courante].date, dateStart);
        }
        else if (dans_sauvegarde)
        {
            if (strncmp(ligne, "Nom:", 4) == 0)
            {
                strcpy(sauvegardes[sauvegarde_courante].nom, ligne + 4);
            }
            else if (strncmp(ligne, "Score:", 6) == 0)
            {
                sauvegardes[sauvegarde_courante].score = atoi(ligne + 6);
            }
            else if (strncmp(ligne, "Vie:", 4) == 0)
            {
                sauvegardes[sauvegarde_courante].vie = atoi(ligne + 4);
                dans_sauvegarde = 0; 
            }
        }
    }

    fclose(fichier);

    if (nbSauvegardes == 0)
    {
        printf("Aucune sauvegarde disponible.\n");
        Sleep(1000);
        return 0;
    }

    printf("+------+--------------------+--------+------+-----------------------------+\n");
    printf("| #    | Nom                | Score  | Vie  | Date                        |\n");
    printf("+------+--------------------+--------+------+-----------------------------+\n");
    for (int i = 0; i < nbSauvegardes; i++)
    {
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
    while (choix < 1 || choix > nbSauvegardes)
    {
        printf("Numero invalide. Entrez un numero valide: ");
        scanf("%d", &choix);
    }

    fichier = fopen("sauvegarde.txt", "r");
    int sauvegarde_trouvee = 0;
    int sauvegarde_index = 1;

    while (fgets(ligne, sizeof(ligne), fichier) && !sauvegarde_trouvee)
    {
        if (strncmp(ligne, "===== Sauvegarde", 16) == 0)
        {
            if (sauvegarde_index == choix)
            {
                sauvegarde_trouvee = 1;

                while (fgets(ligne, sizeof(ligne), fichier))
                {
                    ligne[strcspn(ligne, "\n")] = 0;

                    if (ligne[0] == '\0' || strlen(ligne) == 0)
                    {
                        break;
                    }

                    if (strncmp(ligne, "Nom:", 4) == 0)
                    {
                        strcpy(perso->nom, ligne + 4);
                    }
                    else if (strncmp(ligne, "PositionX:", 10) == 0)
                    {
                        perso->positionX = atoi(ligne + 10);
                    }
                    else if (strncmp(ligne, "PositionY:", 10) == 0)
                    {
                        perso->positionY = atoi(ligne + 10);
                    }
                    else if (strncmp(ligne, "Score:", 6) == 0)
                    {
                        perso->score = atoi(ligne + 6);
                    }
                    else if (strncmp(ligne, "Vie:", 4) == 0)
                    {
                        perso->vie = atoi(ligne + 4);
                    }
                }
            }
            else
            {
                sauvegarde_index++;
            }
        }
    }

    fclose(fichier);

    if (!sauvegarde_trouvee)
    {
        printf("Erreur lors du chargement de la sauvegarde!\n");
        Sleep(1500);
        return 0;
    }

    printf("Partie chargee avec succes !\n");
    Sleep(1500);
    return 1;
}

void resetScores()
{
    printf("Voulez-vous vraiment supprimer toutes les sauvegardes ? (o/n) : ");
    char choix;
    scanf(" %c", &choix);

    if (choix == 'o' || choix == 'O')
    {
        FILE *fichier = fopen("sauvegarde.txt", "w");
        if (fichier != NULL)
        {
            fclose(fichier);
            printf("Les scores ont ete reinitialises avec succes !\n");
        }
        else
        {
            printf("Erreur: Impossible d'ouvrir le fichier de sauvegarde\n");
        }
        system("del temp_*.txt");
        printf("Les fichiers temporaires ont ete supprimes avec succes !\n");
        Sleep(1500);
    }
}

void afficherScores()
{
    FILE *fichier = fopen("sauvegarde.txt", "r");
    if (fichier == NULL)
    {
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

    while (fgets(ligne, sizeof(ligne), fichier))
    {
        if (strstr(ligne, "===== Sauvegarde") == ligne)
        {
            char *start = ligne + 16;
            char *end = strstr(start, " =====");
            if (end)
            {
                *end = '\0';
                strcpy(date_temp, start);
            }
        }
        else if (sscanf(ligne, "Nom:%s", nom_temp) == 1)
        {
        }
        else if (sscanf(ligne, "Score:%d", &score_temp) == 1)
        {
            strcpy(sauvegardes[nbSauvegardes].nom, nom_temp);
            sauvegardes[nbSauvegardes].score = score_temp;
            strcpy(sauvegardes[nbSauvegardes].date, date_temp);
            nbSauvegardes++;
        }
        else if (sscanf(ligne, "Vie:%d", &sauvegardes[nbSauvegardes - 1].vie) == 1)
        {
        }
    }

    fclose(fichier);

    for (int i = 0; i < nbSauvegardes - 1; i++)
    {
        for (int j = 0; j < nbSauvegardes - i - 1; j++)
        {
            if (sauvegardes[j].score < sauvegardes[j + 1].score)
            {
                Sauvegarde temp = sauvegardes[j];
                sauvegardes[j] = sauvegardes[j + 1];
                sauvegardes[j + 1] = temp;
            }
        }
    }

    printf("+------+--------------------+--------+------+-----------------------------+\n");
    printf("| #    | Nom                | Score  | Vie  | Date                        |\n");
    printf("+------+--------------------+--------+------+-----------------------------+\n");

    for (int i = 0; i < nbSauvegardes && i < 10; i++)
    {
        printf("| %3d  | %-18s | %6d | %4d | %-27s |\n",
               i + 1,
               sauvegardes[i].nom,
               sauvegardes[i].score,
               sauvegardes[i].vie,
               sauvegardes[i].date);
    }
    printf("+------+--------------------+--------+------+-----------------------------+\n");

}