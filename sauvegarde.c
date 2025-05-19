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
        int scoresNiveaux[MAX_NIVEAUX]; 
        int vie;
        int niveauMax;
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
                if (currentSave >= 0 && nbSauvegardes < 100) {
                    nbSauvegardes++;
                }
                currentSave = nbSauvegardes;
                
                char *dateStart = ligne + 16;
                strcpy(sauvegardes[currentSave].date, dateStart);
                char *endMarker = strstr(sauvegardes[currentSave].date, " =====");
                if (endMarker)
                    *endMarker = '\0';
                
                sauvegardes[currentSave].positionX = 0;
                sauvegardes[currentSave].positionY = 0;
                for (int i = 0; i < MAX_NIVEAUX; i++) {
                    sauvegardes[currentSave].scoresNiveaux[i] = 0;
                }
                sauvegardes[currentSave].vie = 3;
                sauvegardes[currentSave].niveauMax = 0;
                sauvegardes[currentSave].nom[0] = '\0';
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
            else if (strncmp(ligne, "ScoreNiveau", 11) == 0)
            {
                int niveau = ligne[11] - '0';
                if (niveau >= 0 && niveau < MAX_NIVEAUX) {
                    sauvegardes[currentSave].scoresNiveaux[niveau] = atoi(strchr(ligne, ':') + 1);
                }
            }
            else if (strncmp(ligne, "Vie:", 4) == 0)
            {
                sauvegardes[currentSave].vie = atoi(ligne + 4);
            }
            else if (strncmp(ligne, "NiveauMax:", 10) == 0)
            {
                sauvegardes[currentSave].niveauMax = atoi(ligne + 10);
            }
        }
        
        if (currentSave >= 0 && nbSauvegardes < 100) {
            nbSauvegardes++;
        }
        
        fclose(fichier);
    }

    time_t now;
    time(&now);
    char *date = ctime(&now);
    date[strcspn(date, "\n")] = '\0';
    extern int niveauMaxDebloque;
    extern int niveauActuel;

    if (joueurExiste >= 0)
    {
        sauvegardes[joueurExiste].positionX = perso->positionX;
        sauvegardes[joueurExiste].positionY = perso->positionY;
        
        sauvegardes[joueurExiste].scoresNiveaux[niveauActuel] = perso->score;
        
        sauvegardes[joueurExiste].vie = perso->vie;
        
        if (niveauMaxDebloque > sauvegardes[joueurExiste].niveauMax) {
            sauvegardes[joueurExiste].niveauMax = niveauMaxDebloque;
        }
        strcpy(sauvegardes[joueurExiste].date, date);
    }
    else
    {
        if (nbSauvegardes < 100) {
            strcpy(sauvegardes[nbSauvegardes].nom, perso->nom);
            initGameAudio();
            sauvegardes[nbSauvegardes].positionX = perso->positionX;
            sauvegardes[nbSauvegardes].positionY = perso->positionY;
            
            for (int i = 0; i < MAX_NIVEAUX; i++) {
                sauvegardes[nbSauvegardes].scoresNiveaux[i] = 0;
            }
            sauvegardes[nbSauvegardes].scoresNiveaux[niveauActuel] = perso->score;
            
            sauvegardes[nbSauvegardes].vie = perso->vie;
            sauvegardes[nbSauvegardes].niveauMax = niveauMaxDebloque;
            strcpy(sauvegardes[nbSauvegardes].date, date);
            nbSauvegardes++;
        }
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
        fprintf(fichier, "NiveauMax:%d\n", sauvegardes[i].niveauMax);
        fprintf(fichier, "PositionX:%d\n", sauvegardes[i].positionX);
        fprintf(fichier, "PositionY:%d\n", sauvegardes[i].positionY);
        
        for (int j = 0; j < MAX_NIVEAUX; j++) {
            fprintf(fichier, "ScoreNiveau%d:%d\n", j, sauvegardes[i].scoresNiveaux[j]);
        }
        
        fprintf(fichier, "Vie:%d\n\n", sauvegardes[i].vie);
    }

    fclose(fichier);
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

void sauvegarderProgression(int nouveauNiveauDebloque, const char* nomJoueur, int scoreNiveau)
{
    extern int niveauMaxDebloque;
    extern int niveauActuel;
    
    if (nouveauNiveauDebloque > niveauMaxDebloque) {
        niveauMaxDebloque = nouveauNiveauDebloque;
    }
    
    SauvegardeInfo saves[100];
    int nbSaves = lireSauvegardesExistant(saves, 100);
    int joueurExiste = 0;
    extern char nomJoueurStocke[100];
    
    for(int i = 0; i < nbSaves; i++) {
        if(strcmp(saves[i].nom, nomJoueur) == 0) {
            if(nouveauNiveauDebloque > saves[i].niveauMaxDebloque) {
                saves[i].niveauMaxDebloque = nouveauNiveauDebloque;
            }
            
            if(scoreNiveau > saves[i].scoresNiveaux[niveauActuel]) {
                saves[i].scoresNiveaux[niveauActuel] = scoreNiveau;
            }
            
            int totalScore = 0;
            for(int j = 0; j < MAX_NIVEAUX; j++) {
                totalScore += saves[i].scoresNiveaux[j];
            }
            saves[i].score = totalScore;
            
            if (strlen(nomJoueurStocke) == 0) {
                strcpy(nomJoueurStocke, nomJoueur);
            }
            joueurExiste = 1;
            break;
        }
    }
    
    if (!joueurExiste && nbSaves < 100 && strlen(nomJoueur) > 0) {
        extern int SPAWN_X;
        extern int SPAWN_Y;
        strcpy(saves[nbSaves].nom, nomJoueur);
        saves[nbSaves].niveauMaxDebloque = nouveauNiveauDebloque;
        
        for(int j = 0; j < MAX_NIVEAUX; j++) {
            saves[nbSaves].scoresNiveaux[j] = 0;
        }
        
        saves[nbSaves].scoresNiveaux[niveauActuel] = scoreNiveau;
        
        int totalScore = 0;
        for(int j = 0; j < MAX_NIVEAUX; j++) {
            totalScore += saves[nbSaves].scoresNiveaux[j];
        }
        saves[nbSaves].score = totalScore;
        
        saves[nbSaves].vie = 3;
        saves[nbSaves].positionX = SPAWN_X;
        saves[nbSaves].positionY = SPAWN_Y;
        nbSaves++;
        
        
        if (strlen(nomJoueurStocke) == 0) {
            strcpy(nomJoueurStocke, nomJoueur);
        }
    }
    
    FILE *fichier = fopen("sauvegarde.txt", "w");
    if (fichier) {
        for(int i = 0; i < nbSaves; i++) {
            
            time_t now;
            time(&now);
            char *date = ctime(&now);
            date[strcspn(date, "\n")] = '\0';
            
            fprintf(fichier, "===== Sauvegarde %s =====\n", date);
            fprintf(fichier, "Nom:%s\n", saves[i].nom);
            fprintf(fichier, "NiveauMax:%d\n", saves[i].niveauMaxDebloque);
            fprintf(fichier, "Vie:%d\n", saves[i].vie);
            fprintf(fichier, "PositionX:%d\n", saves[i].positionX);
            fprintf(fichier, "PositionY:%d\n", saves[i].positionY);
            
            for(int j = 0; j < MAX_NIVEAUX; j++) {
                fprintf(fichier, "ScoreNiveau%d:%d\n", j, saves[i].scoresNiveaux[j]);
            }
            
            fprintf(fichier, "\n");
        }
        fclose(fichier);
    }
}

int lireSauvegarde()
{
    SauvegardeInfo saves[100];
    int nbSaves = lireSauvegardesExistant(saves, 100);
    extern int niveauMaxDebloque;
    extern char nomJoueurStocke[100];
    
    if (strlen(nomJoueurStocke) > 0) {
        for (int i = 0; i < nbSaves; i++) {
            if (strcmp(saves[i].nom, nomJoueurStocke) == 0) {
                
                niveauMaxDebloque = saves[i].niveauMaxDebloque;
                return saves[i].niveauMaxDebloque;
            }
        }
    }
    
    
    int maxNiveau = 0;
    for (int i = 0; i < nbSaves; i++) {
        if (saves[i].niveauMaxDebloque > maxNiveau) {
            maxNiveau = saves[i].niveauMaxDebloque;
        }
    }
    
    niveauMaxDebloque = maxNiveau;
    return maxNiveau;
}

void resetScores()
{
    system("cls");
    extern char nomJoueurStocke[100];
    extern int niveauActuel;
    extern int niveauMaxDebloque;
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
            return;
        }
        system("del temp_*.txt");
        niveauActuel = 0;
        niveauMaxDebloque = 0;
        nomJoueurStocke[0] = '\0';
        printf("Les fichiers temporaires ont ete supprimes avec succes !\n");
        Sleep(1500);
    }
}

void initialiserNiveaux()
{
    extern Niveau niveaux[MAX_NIVEAUX];
    extern int niveauMaxDebloque;

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

void mettreAJourCoordonnees(int *x, int *y, int *yMort) {
    extern int niveauActuel;
    
    switch (niveauActuel) {
        case 0:
            *x = 21;
            *y = 15;
            *yMort = 23;
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
            *yMort = 20;
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

    typedef struct {
        char nom[100];
        int score;
        char date[100];
        int vie;
        int scoresNiveaux[MAX_NIVEAUX];
    } ScoreAffichage;
    
    ScoreAffichage scores[100];
    int nbScores = 0;
    char ligne[100];
    int scoreIndex = -1;
    
    while (fgets(ligne, sizeof(ligne), fichier))
    {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strstr(ligne, "===== Sauvegarde") == ligne)
        {
            if (scoreIndex >= 0) {
                int totalScore = 0;
                for (int i = 0; i < MAX_NIVEAUX; i++) {
                    totalScore += scores[scoreIndex].scoresNiveaux[i];
                }
                scores[scoreIndex].score = totalScore;
                nbScores++;
            }
            
            scoreIndex = nbScores;
            
            char *start = ligne + 16;
            char *end = strstr(start, " =====");
            if (end)
            {
                *end = '\0';
                strcpy(scores[scoreIndex].date, start);
            }
            
            scores[scoreIndex].score = 0;
            for (int i = 0; i < MAX_NIVEAUX; i++) {
                scores[scoreIndex].scoresNiveaux[i] = 0;
            }
        }
        else if (strncmp(ligne, "ScoreNiveau", 11) == 0)
        {
            int niveau = ligne[11] - '0';
            if(niveau >= 0 && niveau < MAX_NIVEAUX) {
                scores[scoreIndex].scoresNiveaux[niveau] = atoi(strchr(ligne, ':') + 1);
            }
        }
    }
    
    if (scoreIndex >= 0 && scoreIndex == nbScores) {
        int totalScore = 0;
        for (int i = 0; i < MAX_NIVEAUX; i++) {
            totalScore += scores[scoreIndex].scoresNiveaux[i];
        }
        scores[scoreIndex].score = totalScore;
        nbScores++;
    }

    fclose(fichier);

    for (int i = 0; i < nbScores - 1; i++)
    {
        for (int j = 0; j < nbScores - i - 1; j++)
        {
            if (scores[j].score < scores[j + 1].score)
            {
                ScoreAffichage temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    printf("+------+--------------------+--------+------+-----------------------------+\n");
    printf("| #    | Nom                | Score  | Vie  | Date                        |\n");
    printf("+------+--------------------+--------+------+-----------------------------+\n");

    for (int i = 0; i < nbScores && i < 10; i++)
    {
        printf("| %3d  | %-18s | %6d | %4d | %-27s |\n",
               i + 1,
               scores[i].nom,
               scores[i].score,
               scores[i].vie,
               scores[i].date);
    }
    printf("+------+--------------------+--------+------+-----------------------------+\n\n");
    
    printf("Scores par niveau:\n");
    printf("+------+--------------------+");
    for (int n = 0; n < MAX_NIVEAUX; n++) {
        printf("--------+");
    }
    printf("\n");
    
    printf("| #    | Nom                |");
    for (int n = 0; n < MAX_NIVEAUX; n++) {
        printf(" Niv.%-2d |", n+1);
    }
    printf("\n");
    
    printf("+------+--------------------+");
    for (int n = 0; n < MAX_NIVEAUX; n++) {
        printf("--------+");
    }
    printf("\n");
    
    for (int i = 0; i < nbScores && i < 10; i++)
    {
        printf("| %3d  | %-18s |", i + 1, scores[i].nom);
        for (int n = 0; n < MAX_NIVEAUX; n++) {
            printf(" %6d |", scores[i].scoresNiveaux[n]);
        }
        printf("\n");
    }
    
    printf("+------+--------------------+");
    for (int n = 0; n < MAX_NIVEAUX; n++) {
        printf("--------+");
    }
    printf("\n");
}

int lireSauvegardesExistant(SauvegardeInfo *saves, int maxSaves) {
    FILE *fichier = fopen("sauvegarde.txt", "r");
    if (fichier == NULL) return 0;

    int count = 0;
    char ligne[1024];
    SauvegardeInfo current;
    int nouveauJoueur = 1;
    extern int niveauMaxDebloque;

    memset(&current, 0, sizeof(current));
    
    while (fgets(ligne, sizeof(ligne), fichier) && count < maxSaves) {
        ligne[strcspn(ligne, "\n")] = 0;

        if (strstr(ligne, "===== Sauvegarde") == ligne) {
            if (!nouveauJoueur && count < maxSaves) {
                int totalScore = 0;
                for (int j = 0; j < MAX_NIVEAUX; j++) {
                    totalScore += current.scoresNiveaux[j];
                }
                current.score = totalScore;
                
                saves[count++] = current;
            }
            memset(&current, 0, sizeof(current));
            nouveauJoueur = 1;
        }
        else if (strncmp(ligne, "Nom:", 4) == 0) {
            strcpy(current.nom, ligne + 4);
            nouveauJoueur = 0; 
        }
        else if (strncmp(ligne, "NiveauMax:", 10) == 0) {
            current.niveauMaxDebloque = atoi(ligne + 10);
        }
        else if (strncmp(ligne, "ScoreNiveau", 11) == 0) {
            int niveau = ligne[11] - '0';
            if(niveau >= 0 && niveau < MAX_NIVEAUX) {
                current.scoresNiveaux[niveau] = atoi(strchr(ligne, ':') + 1);
            }
        }
        else if (strncmp(ligne, "Vie:", 4) == 0) {
            current.vie = atoi(ligne + 4);
        }
        else if (strncmp(ligne, "PositionX:", 10) == 0) {
            current.positionX = atoi(ligne + 10);
        }
        else if (strncmp(ligne, "PositionY:", 10) == 0) {
            current.positionY = atoi(ligne + 10);
        }
    }
    
    if (!nouveauJoueur && count < maxSaves) {
        int totalScore = 0;
        for (int j = 0; j < MAX_NIVEAUX; j++) {
            totalScore += current.scoresNiveaux[j];
        }
        current.score = totalScore;
        
        saves[count++] = current;
    }

    fclose(fichier);
    return count;
}