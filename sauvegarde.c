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
    menuPrincipal(niveaux);
}

void chargerProgression(Progression* progression) {
    FILE* f = fopen("progression.dat", "rb");
    if (f) {
        fread(progression->niveauxDebloques, sizeof(int), MAX_NIVEAUX, f);
        fclose(f);
    } else {
        for (int i = 0; i < MAX_NIVEAUX; i++) progression->niveauxDebloques[i] = (i == 0 ? 1 : 0); 
    }
}

void sauvegarderProgression(int nouveauNiveauDebloque, const char* nomJoueur)
{
    SauvegardeInfo saves[100];
    int nbSaves = lireSauvegardesExistant(saves, 100);
    
    // Mettre à jour la progression pour le joueur actuel
    for(int i = 0; i < nbSaves; i++) {
        if(strcmp(saves[i].nom, nomJoueur) == 0) {
            if(nouveauNiveauDebloque > saves[i].niveauMaxDebloque) {
                saves[i].niveauMaxDebloque = nouveauNiveauDebloque;
            }
            break;
        }
    }
    
    // Réécrire toutes les sauvegardes
    FILE *fichier = fopen("sauvegarde.txt", "w");
    if (fichier) {
        for(int i = 0; i < nbSaves; i++) {
            fprintf(fichier, "===== Sauvegarde %s =====\n", saves[i].nom);
            fprintf(fichier, "Nom:%s\n", saves[i].nom);
            fprintf(fichier, "NiveauMax:%d\n", saves[i].niveauMaxDebloque);
            fprintf(fichier, "Score:%d\n", saves[i].score);
            fprintf(fichier, "Vie:%d\n", saves[i].vie);
            fprintf(fichier, "PositionX:%d\n", saves[i].positionX);
            fprintf(fichier, "PositionY:%d\n\n", saves[i].positionY);
        }
        fclose(fichier);
    }
}

int lireSauvegarde()
{
    FILE *fichier = fopen("sauvegarde/progression.dat", "rb");
    int niveauMaxDebloque = 0;
    
    if (fichier != NULL) {
        fread(&niveauMaxDebloque, sizeof(int), 1, fichier);
        fclose(fichier);
    }
    
    return niveauMaxDebloque;
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
        nomJoueurStocke[0] = '\0';
        printf("Les fichiers temporaires ont ete supprimes avec succes !\n");
        Sleep(1500);
    }
}

void initialiserNiveaux(Niveau *niveaux, int niveauMaxDebloque)
{
    strcpy(niveaux[0].fichier, "Mario1.txt");
    strcpy(niveaux[1].fichier, "Mario2.txt");
    strcpy(niveaux[2].fichier, "Mario3.txt");
    strcpy(niveaux[3].fichier, "Mario4.txt");
    strcpy(niveaux[4].fichier, "Mario5.txt");

    niveaux[0].debloque = 1;
    
    for (int i = 1; i < MAX_NIVEAUX; i++) {
        niveaux[i].debloque = (i <= niveauMaxDebloque) ? 1 : 0;
    }

    for (int i = 0; i < MAX_NIVEAUX; i++) {
        niveaux[i].termine = 0;
    }
}

void mettreAJourCoordonnees(int niveauActuel, int *x, int *y, int *yMort) {
    switch (niveauActuel) {
        case 0:
            *x = 21;
            *y = 15;
            *yMort = 19;
            break;
        case 1:
            *x = 21;
            *y = 15;
            *yMort = 23;
            break;
        case 2:
            *x = 21;
            *y = 15;
            *yMort = 20;
            break;
        case 3:
            *x = 21;
            *y = 15;
            *yMort = 19;
            break;
        case 4:
            *x = 21;
            *y = 15;
            *yMort = 19;
            break;
        default:
            break;
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

// Nouvelle fonction pour lire les sauvegardes existantes
int lireSauvegardesExistant(SauvegardeInfo *saves, int maxSaves) {
    FILE *fichier = fopen("sauvegarde.txt", "r");
    if (fichier == NULL) return 0;

    int count = 0;
    char ligne[1024];
    SauvegardeInfo current = {0};

    while (fgets(ligne, sizeof(ligne), fichier) && count < maxSaves) {
        ligne[strcspn(ligne, "\n")] = 0;

        if (strstr(ligne, "===== Sauvegarde")) {
            memset(&current, 0, sizeof(current));
        }
        else if (strncmp(ligne, "Nom:", 4) == 0) {
            strcpy(current.nom, ligne + 4);
        }
        else if (strncmp(ligne, "Score:", 6) == 0) {
            current.score = atoi(ligne + 6);
        }
        else if (strncmp(ligne, "Vie:", 4) == 0) {
            current.vie = atoi(ligne + 4);
        }
        else if (strncmp(ligne, "PositionX:", 10) == 0) {
            current.positionX = atoi(ligne + 10);
        }
        else if (strncmp(ligne, "PositionY:", 10) == 0) {
            current.positionY = atoi(ligne + 10);
            // Dernière information de la sauvegarde
            saves[count++] = current;
        }
    }

    fclose(fichier);
    return count;
}