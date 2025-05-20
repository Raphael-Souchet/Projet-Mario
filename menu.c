#include "projet.h"

void setCouleur(int couleur)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, couleur);
}

int afficherOption(int numero, const char *texte, int selection_actuelle)
{
    if (selection_actuelle == numero)
    {
        setCouleur(240);
        printf("|              %d. %-30s |\n", numero, texte);
        setCouleur(7);
    }
    else
    {
        printf("|              %d. %-30s |\n", numero, texte);
    }
}

int navigationMenu(int selection, int min, int max, int touche, Niveau *niveaux)
{
    int nouvelleSelection = selection;

    if (touche == 72 || touche == 122 || touche == 90)
        nouvelleSelection = (nouvelleSelection > min) ? nouvelleSelection - 1 : max;
    else if (touche == 80 || touche == 115 || touche == 83)
        nouvelleSelection = (nouvelleSelection < max) ? nouvelleSelection + 1 : min;

    if (niveaux && nouvelleSelection >= 1 && nouvelleSelection <= MAX_NIVEAUX && !niveaux[nouvelleSelection - 1].debloque)
        return navigationMenu(nouvelleSelection, min, max, touche, niveaux);

    return nouvelleSelection;
}

void afficherMenuPrincipal(int selection, Niveau *niveaux)
{
    extern char nomJoueurStocke[100];

    int totalScore = 0;
    if (strlen(nomJoueurStocke) > 0)
    {
        SauvegardeInfo saves[100];
        int nbSaves = lireSauvegardesExistant(saves, 100);

        for (int i = 0; i < nbSaves; i++)
        {
            if (strcmp(saves[i].nom, nomJoueurStocke) == 0)
            {
                for(int j = 0; j < MAX_NIVEAUX; j++) {
                    totalScore += saves[i].scoresNiveaux[j];
                }
                break;
            }
        }
    }

    system("cls");
    printf("+---------------------------------------------+\n");
    printf("|     Bienvenue dans l'univers de Mario !     |\n");
    printf("|                   Score : %-5d             |\n", totalScore);
    printf("|                                             |\n");
    printf("|                   - MENU -                  |\n");
    printf("|                                             |\n");

    for (int i = 0; i < MAX_NIVEAUX; i++)
    {
        if (selection == i + 1)
            setCouleur(240);
        else if (!niveaux[i].debloque)
            setCouleur(8);

        if (niveaux[i].debloque)
            printf("|             Niveau %-2d : %-15s     |\n", i + 1, "Debloque");
        else
            printf("|             Niveau %-2d : %-15s     |\n", i + 1, "Verrouille");

        setCouleur(7);
    }

    if (selection == MAX_NIVEAUX + 1)
        setCouleur(240);
    printf("|                Scores                       |\n");
    setCouleur(7);

    if (selection == MAX_NIVEAUX + 2)
        setCouleur(240);
    printf("|                Reinitialiser                |\n");
    setCouleur(7);

    if (selection == MAX_NIVEAUX + 3)
        setCouleur(240);
    printf("|                Quitter                      |\n");
    setCouleur(7);

    printf("+---------------------------------------------+\n");
}

int menuPauseEnJeu()
{
    int selection = 1, touche;

    while (_kbhit())
        _getch();

    while (1)
    {
        system("cls");
        printf("+---------------------------------------------+\n");
        printf("|                    PAUSE                    |\n");
        printf("|                                             |\n");

        if (selection == 1)
            setCouleur(240);
        printf("|                  Reprendre                  |\n");
        setCouleur(7);

        if (selection == 2)
            setCouleur(240);
        printf("|                Menu Principal               |\n");
        setCouleur(7);

        printf("+---------------------------------------------+\n");

        touche = _getch();
        if (touche == 0 || touche == 224)
            touche = _getch();

        if (touche == 72 || touche == 122 || touche == 90 || touche == 80 || touche == 115 || touche == 83)
            selection = (selection == 1) ? 2 : 1;

        if (touche == 13)
            return selection;
    }
}

void afficherMenuMort(Personnage *perso, int selection)
{
    system("cls");
    if (perso->vie > 0)
    {
        printf("+---------------------------------------------+\n");
        printf("|                VOUS ETES MORT !             |\n");
        printf("|               Vies restantes : %-12d |\n", perso->vie);
        printf("|                                             |\n");

        if (selection == 1)
            setCouleur(240);
        printf("|                  Recommencer                |\n");
        setCouleur(7);

        if (selection == 2)
            setCouleur(240);
        printf("|                 Menu Principal              |\n");
        setCouleur(7);

        printf("+---------------------------------------------+\n");
    }
    else
    {
        printf("+---------------------------------------------+\n");
        printf("|                 GAME OVER !                 |\n");
        printf("|                                             |\n");
        printf("|         Appuyez sur une touche pour         |\n");
        printf("|         retourner au menu principal         |\n");
        printf("+---------------------------------------------+\n");
    }
}

void menu_mort(Personnage *perso, const char *fichierTemp)
{
    extern Niveau niveaux[MAX_NIVEAUX];
    extern int niveauMaxDebloque;
    extern int menuMusicPlaying;
    menuMusicPlaying = 0;
    niveauMaxDebloque = lireSauvegarde();
    initialiserNiveaux(niveaux, niveauMaxDebloque);

    if (perso->vie > 0)
    {
        int selection = 1, touche;
        while (_kbhit())
            _getch();

        while (1)
        {
            afficherMenuMort(perso, selection);

            touche = _getch();
            if (touche == 0 || touche == 224)
                touche = _getch();

            selection = navigationMenu(selection, 1, 2, touche, NULL);

            if (touche == 13)
            {
                stopBackgroundMusic();
                cleanupAudio();

                if (selection == 1)
                {
                    char *musicpath = NULL;
                    switch (niveauActuel)
                    {
                    case 0:
                        musicpath = "asset/music/overworld.mp3";
                        break;
                    case 1:
                        musicpath = "asset/music/sky.mp3";
                        break;
                    case 2:
                        musicpath = "asset/music/beach.mp3";
                        break;
                    default:
                        musicpath = "asset/music/overworld.mp3";
                    }
                    loadBackgroundMusic(musicpath);
                    setMusicVolume(96);
                    playBackgroundMusic(-1);
                    menuMusicPlaying = 1;
                    remove(fichierTemp);
                    extern int niveauMaxDebloque;
                    if (!copierFichier(niveaux[niveauMaxDebloque].fichier, fichierTemp))
                    {
                        Sleep(1500);
                        menuPrincipal(niveaux);
                        return;
                    }

                    extern int SPAWN_X;
                    extern int SPAWN_Y;
                    perso->positionX = SPAWN_X;
                    perso->positionY = SPAWN_Y;
                    perso->score = 0;
                    jouer(fichierTemp, perso);
                    return;
                }
                else
                {
                    menuPrincipal(niveaux);
                    return;
                }
            }
        }
    }
    else
    {
        afficherMenuMort(perso, 0);
        while (_kbhit())
            _getch();
        _getch();
        menuPrincipal(niveaux);
    }
}

void menuVictoire(Personnage *perso)
{
    system("cls");
    int selection = 1, touche;
    extern int niveauActuel;
    extern int niveauMaxDebloque;
    extern Niveau niveaux[MAX_NIVEAUX];

    sauvegarderProgression(niveauMaxDebloque, perso->nom, perso->score);

    if (niveauActuel + 1 < MAX_NIVEAUX && niveauActuel + 1 > niveauMaxDebloque)
    {
        niveaux[niveauActuel + 1].debloque = 1;
        niveauMaxDebloque = niveauActuel + 1;
        sauvegarderProgression(niveauMaxDebloque, perso->nom, perso->score);
    }

    char *fichierTemp = creerNomFichierTemp(perso->nom);
    Carte *carte = chargerCarteEnMemoire(fichierTemp);
    if (carte != NULL)
    {
        sauvegarderPartie(perso, carte, fichierTemp);
        libererCarte(carte);
    }

    stopBackgroundMusic();
    loadBackgroundMusic("asset/music/sky.mp3");
    setMusicVolume(96);
    playBackgroundMusic(-1);
    menuMusicPlaying = 1;

    while (_kbhit())
        _getch();

    while (1)
    {
        system("cls");
        printf("+---------------------------------------------+\n");
        printf("|                VOUS AVEZ GAGNE !            |\n");
        printf("|                Score final : %-14d |\n", perso->score);
        printf("|                                             |\n");

        if (selection == 1)
            setCouleur(240);
        printf("|             Retour au menu principal         |\n");
        setCouleur(7);

        printf("+---------------------------------------------+\n");

        touche = _getch();
        if (touche == 0 || touche == 224)
            touche = _getch();

        selection = navigationMenu(selection, 1, 1, touche, NULL);

        if (touche == 13)
        {
            if (selection == 1 && niveauActuel + 1 < MAX_NIVEAUX && niveaux[niveauActuel + 1].debloque)
            {
                stopBackgroundMusic();
                menuMusicPlaying = 0;

                remove(fichierTemp);
                free(fichierTemp);

                fichierTemp = creerNomFichierTemp(perso->nom);
                if (!copierFichier(niveaux[niveauActuel + 1].fichier, fichierTemp))
                {
                    Sleep(1500);
                    free(fichierTemp);
                    menuPrincipal(niveaux);
                    return;
                }
                niveauActuel++;
                extern int SPAWN_X;
                extern int SPAWN_Y;
                extern int MORT_Y;
                mettreAJourCoordonnees(&SPAWN_X, &SPAWN_Y, &MORT_Y);
                perso->positionX = SPAWN_X;
                perso->positionY = SPAWN_Y;
                perso->score = 0;
                jouer(fichierTemp, perso);
                free(fichierTemp);
                return;
            }
            else
            {
                free(fichierTemp);
                menuPrincipal(niveaux);
                return;
            }
        }
    }
}

void menuPrincipal()
{
    extern int SPAWN_X;
    extern int SPAWN_Y;
    extern int niveauMaxDebloque;
    extern Niveau niveaux[MAX_NIVEAUX];
    Personnage perso = {SPAWN_X, SPAWN_Y, "", 0, 0, 0, 1, 1, 1, 3};
    int selection = 1, touche;
    char *fichierTemp = NULL;
    extern int niveauActuel;
    extern char nomJoueurStocke[100];

    if (!menuMusicPlaying)
    {
        cleanupAudio();
        if (initAudio())
        {
            loadBackgroundMusic("asset/music/ost.mp3");
            setMusicVolume(96);
            playBackgroundMusic(-1);
            menuMusicPlaying = 1;
        }
    }

    if (strlen(nomJoueurStocke) > 0)
    {
        strcpy(perso.nom, nomJoueurStocke);
    }

    static Niveau niveauxLocaux[MAX_NIVEAUX];
    if (niveaux == NULL)
    {
        niveauMaxDebloque = lireSauvegarde();
        initialiserNiveaux(niveauxLocaux, niveauMaxDebloque);
        memcpy(niveaux, niveauxLocaux, sizeof(niveauxLocaux));
    }
    else
    {

        for (int i = 0; i < MAX_NIVEAUX; i++)
        {
            niveaux[i].debloque = (i <= niveauMaxDebloque) ? 1 : 0;
        }
    }

    while (_kbhit())
        _getch();

    while (1)
    {
        int totalScore = 0;
        afficherMenuPrincipal(selection, niveaux);

        touche = _getch();
        if (touche == 0 || touche == 224)
            touche = _getch();

        selection = navigationMenu(selection, 1, MAX_NIVEAUX + 3, touche, niveaux);

        if (touche == 13)
        {
            if (selection >= 1 && selection <= MAX_NIVEAUX && niveaux[selection - 1].debloque)
            {
                stopBackgroundMusic();
                menuMusicPlaying = 0;

                if (strlen(perso.nom) == 0)
                {
                    system("cls");
                    printf("Entrez votre nom: ");
                    scanf("%s", perso.nom);
                    strcpy(nomJoueurStocke, perso.nom);

                    SauvegardeInfo saves[100];
                    int nbSaves = lireSauvegardesExistant(saves, 100);
                    for (int i = 0; i < nbSaves; i++)
                    {
                        if (strcmp(saves[i].nom, perso.nom) == 0)
                        {
                            niveauMaxDebloque = saves[i].niveauMaxDebloque;
                            perso.vie = saves[i].vie;
                            break;
                        }
                    }

                    for (int i = 0; i < MAX_NIVEAUX; i++)
                    {
                        niveaux[i].debloque = (i <= niveauMaxDebloque) ? 1 : 0;
                    }
                }

                fichierTemp = creerNomFichierTemp(perso.nom);
                remove(fichierTemp);

                if (!copierFichier(niveaux[selection - 1].fichier, fichierTemp))
                {
                    printf("Erreur de chargement du niveau\n");
                    Sleep(1500);
                    free(fichierTemp);
                    continue;
                }

                niveauActuel = selection - 1;
                extern int MORT_Y;
                mettreAJourCoordonnees(&SPAWN_X, &SPAWN_Y, &MORT_Y);
                perso.positionX = SPAWN_X;
                perso.positionY = SPAWN_Y;
                perso.score = 0; 

                jouer(fichierTemp, &perso);
                free(fichierTemp);
                return;
            }
            else if (selection == MAX_NIVEAUX + 1)
            {
                system("cls");
                afficherScores();
                printf("\nAppuyez sur une touche pour revenir...");
                _getch();
            }
            else if (selection == MAX_NIVEAUX + 2)
            {
                resetScores();
                extern Niveau niveaux[MAX_NIVEAUX];
                initialiserNiveaux(niveaux, 0);

                printf("Scores et progression reinitialises !\n");
                Sleep(1500);
                system("cls");
                menuPrincipal();
            }
            else if (selection == MAX_NIVEAUX + 3)
            {
                system("cls");
                printf("Merci d'avoir joue !\n");
                Sleep(1500);
                system("cls");

                stopBackgroundMusic();
                cleanupAudio();

                exit(0);
            }
        }
    }
}