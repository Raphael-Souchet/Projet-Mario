#include "projet.h"

void menuSauvegarde(Personnage *perso, FILE *fichier)
{
    int choix;
    char *nouveauFichierTemp;
    char *fichierTemp = creerNomFichierTemp(perso->nom);

    while (1)
    {
        system("cls");
        printf("+---------------------------------------------+\n");
        printf("|               Menu Sauvegarde               |\n");
        printf("|                                             |\n");
        printf("|           1. Sauvegarder la partie          |\n");
        printf("|           2. Retour au jeu                  |\n");
        printf("|           3. Menu Principal                 |\n");
        printf("+---------------------------------------------+\n");
        printf("Votre choix: ");
        while (_kbhit())
            _getch();
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            sauvegarderPartie(perso, fichier);
            break;
        case 2:
            if (fichier == NULL)
            {
                fichier = fopen(fichierTemp, "r+");
                if (fichier == NULL)
                {
                    printf("Erreur: Impossible de reouvrir le fichier de jeu\n");
                    Sleep(1500);
                    free(fichierTemp);
                    menuPrincipal("Mario.txt");
                    return;
                }
            }
            free(fichierTemp);
            return;
        case 3:
            if (fichier != NULL)
            {
                fclose(fichier);
            }
            remove(fichierTemp);
            free(fichierTemp);
            menuPrincipal("Mario.txt");
            return;
        default:
            printf("Choix invalide !\n");
            Sleep(1000);
            break;
        }
    }
}

void menuPrincipal(const char *fichierOriginal)
{
    Personnage perso = {SPAWN_X, SPAWN_Y, "", 0, 0, 0, 1, 1, 1, 3};
    int choix;
    char *fichierTemp = NULL;

    system("cls");
    printf("+---------------------------------------------+\n");
    printf("|     Bienvenue sur le meilleur jeu Mario     |\n");
    printf("|                                             |\n");
    printf("|                   - Menu -                  |\n");
    printf("|                                             |\n");
    printf("|              1. Nouvelle Partie             |\n");
    printf("|              2. Charger Partie              |\n");
    printf("|              3. Score                       |\n");
    printf("|              4. Reset les scores            |\n");
    printf("|              5. Quitter                     |\n");
    printf("+---------------------------------------------+\n");
    printf("Choisissez une option : ");
    scanf("%d", &choix);

    if (choix < 1 || choix > 5)
    {
        printf("Choix invalide, veuillez reessayer\n");
        Sleep(1000);
        menuPrincipal(fichierOriginal);
        return;
    }

    system("cls");

    switch (choix)
    {
    case 1:
        printf("Entrez votre nom: ");
        scanf("%s", perso.nom);

        FILE *fichier_verif = fopen("sauvegarde.txt", "r");
        int nom_existe = 0;

        if (fichier_verif != NULL)
        {
            char ligne[100], nom_check[100];
            while (fgets(ligne, sizeof(ligne), fichier_verif))
            {
                if (sscanf(ligne, "Nom:%s", nom_check) == 1)
                {
                    if (strcmp(nom_check, perso.nom) == 0)
                    {
                        nom_existe = 1;
                        break;
                    }
                }
            }
            fclose(fichier_verif);
        }

        if (nom_existe)
        {
            printf("Un joueur avec ce nom existe deja. Voulez-vous continuer? (o/n): ");
            char choix;
            scanf(" %c", &choix);
            if (choix != 'o' && choix != 'O')
            {
                menuPrincipal(fichierOriginal);
                return;
            }
        }

        fichierTemp = creerNomFichierTemp(perso.nom);

        remove(fichierTemp);

        if (!copierFichier(fichierOriginal, fichierTemp))
        {
            printf("Erreur lors de la copie du fichier original\n");
            Sleep(1500);
            free(fichierTemp);
            menuPrincipal(fichierOriginal);
            return;
        }

        jouer(fichierTemp, &perso);
        free(fichierTemp);
        break;
    case 2:
        if (chargerPartie(&perso))
        {
            fichierTemp = creerNomFichierTemp(perso.nom);

            FILE *test = fopen(fichierTemp, "r");
            if (test)
            {
                fclose(test);
                jouer(fichierTemp, &perso);
            }
            else
            {
                printf("Erreur: Fichier de jeu temporaire manquant !\n");
                Sleep(1500);
            }
            free(fichierTemp);
        }
        else
        {
            menuPrincipal(fichierOriginal);
        }
        break;
    case 3:
        afficherScores();
        menuPrincipal(fichierOriginal);
        break;
    case 4:
        resetScores();
        menuPrincipal(fichierOriginal);
        break;
    case 5:
        exit(0);
    }
}

void menu_mort(Personnage *perso, const char *fichierTemp)
{
    system("cls");
    if (perso->vie > 0)
    {
        printf("+---------------------------------------------+\n");
        printf("|                 VOUS ETES MORT!             |\n");
        printf("|               Vies restantes : %d            |\n", perso->vie);
        printf("|                                             |\n");
        printf("|                1. Recommencer               |\n");
        printf("|               2. Menu Principal             |\n");
        printf("+---------------------------------------------+\n");
        printf("Choix : ");
        while (_kbhit())
            _getch();
        int choix;
        scanf("%d", &choix);

        if (choix == 1)
        {
            remove(fichierTemp);
            if (!copierFichier("Mario.txt", fichierTemp))
            {
                printf("Erreur lors de la reinitialisation du niveau !\n");
                Sleep(1500);
                menuPrincipal("Mario.txt");
                return;
            }
            perso->positionX = SPAWN_MORT_X;
            perso->positionY = SPAWN_Y;
            perso->score = 0;
            jouer(fichierTemp, perso);
        }
        else
        {
            menuPrincipal("Mario.txt");
        }
    }
    else
    {
        printf("+---------------------------------------------+\n");
        printf("|                 GAME OVER !                 |\n");
        printf("|                                             |\n");
        printf("|         Appuyez sur une touche pour         |\n");
        printf("|         retourner au menu principal         |\n");
        printf("+---------------------------------------------+\n");
        while (_kbhit())
            _getch();
        _getch();
        menuPrincipal("Mario.txt");
    }
}