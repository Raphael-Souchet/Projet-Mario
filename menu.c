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
    system("cls");
    printf("+---------------------------------------------+\n");
    printf("|     Bienvenue dans l'univers de Mario !     |\n");
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

void afficherMenuSauvegarde(int selection)
{
    system("cls");
    printf("+---------------------------------------------+\n");
    printf("|               Menu Sauvegarde               |\n");
    printf("|                                             |\n");
    
    if (selection == 1)
        setCouleur(240);
    printf("|             Sauvegarder la partie           |\n");
    setCouleur(7);
    
    if (selection == 2)
        setCouleur(240);
    printf("|             Retour au jeu                   |\n");
    setCouleur(7);
    
    if (selection == 3)
        setCouleur(240);
    printf("|             Menu Principal                  |\n");
    setCouleur(7);
    
    printf("+---------------------------------------------+\n");
}

void menuSauvegarde(Personnage *perso, Carte *carte, Niveau *niveaux)
{
    char *fichierTemp = creerNomFichierTemp(perso->nom);
    int selection = 1;
    int touche;
    
    while (_kbhit())
        _getch();
    
    while (1)
    {
        afficherMenuSauvegarde(selection);
        
        touche = _getch();
        
        if (touche == 0 || touche == 224)
            touche = _getch();
        
        selection = navigationMenu(selection, 1, 3, touche, NULL);
        
        if (touche == 13)
        {
            switch (selection)
            {
                case 1:
                    sauvegarderPartie(perso, carte, fichierTemp);
                    printf("Partie sauvegardee avec succes!\n");
                    Sleep(1500);
                    break;
                case 2:
                    jouer(fichierTemp, perso, niveaux);
                    free(fichierTemp);
                    return;
                case 3:
                    sauvegarderCarteVersFichier(carte, fichierTemp);
                    libererCarte(carte);
                    free(fichierTemp);
                    menuPrincipal(niveaux);
                    return;
            }
        }
    }
}

void afficherMenuPauseEnJeu()
{
    system("cls");
    printf("+---------------------------------------------+\n");
    printf("|                 PAUSE                       |\n");
    printf("|                                             |\n");
    printf("|                Reprendre                    |\n");
    printf("|               Menu Principal                |\n");
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
        printf("|                 PAUSE                       |\n");
        printf("|                                             |\n");
        
        if (selection == 1)
            setCouleur(240);
        printf("|                Reprendre                    |\n");
        setCouleur(7);
        
        if (selection == 2)
            setCouleur(240);
        printf("|               Menu Principal                |\n");
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
        printf("|                 VOUS ÊTES MORT !            |\n");
        printf("|            Vies restantes : %-15d |\n", perso->vie);
        printf("|                                             |\n");

        if (selection == 1)
            setCouleur(240);
        printf("|               Recommencer                   |\n");
        setCouleur(7);

        if (selection == 2)
            setCouleur(240);
        printf("|              Menu Principal                 |\n");
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

void menu_mort(Personnage *perso, const char *fichierTemp, int niveauActuel)
{
    Niveau niveaux[MAX_NIVEAUX];
    int niveauMaxDebloque = lireSauvegarde();
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
                if (selection == 1)
                {
                    remove(fichierTemp);
                    if (!copierFichier(niveaux[niveauActuel].fichier, fichierTemp))
                    {
                        printf("Erreur de reinitialisation !\n");
                        Sleep(1500);
                        menuPrincipal(niveaux);
                        return;
                    }
                    perso->positionX = SPAWN_X;
                    perso->positionY = SPAWN_Y;
                    perso->score = 0;
                    jouer(fichierTemp, perso, niveaux);
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

void menuVictoire(Personnage *perso, Niveau *niveaux, int niveauActuel, int niveauMaxDebloque)
{
    system("cls");
    int selection = 1, touche;

    if (niveauActuel + 1 < MAX_NIVEAUX && niveauActuel + 1 > niveauMaxDebloque)
    {
        niveaux[niveauActuel + 1].debloque = 1;
        niveauMaxDebloque = niveauActuel + 1;
        sauvegarderProgression(niveauMaxDebloque);
    }

    char *fichierTemp = creerNomFichierTemp(perso->nom);
    Carte *carte = chargerCarteEnMemoire(fichierTemp);
    if (carte != NULL) {
        sauvegarderPartie(perso, carte, fichierTemp);
        libererCarte(carte);
    }

    while (_kbhit())
        _getch();

    while (1)
    {
        system("cls");
        printf("+---------------------------------------------+\n");
        printf("|              VOUS AVEZ GAGNE !              |\n");
        printf("|           Score final : %-19d |\n", perso->score);
        printf("|                                             |\n");

        if (niveauActuel + 1 < MAX_NIVEAUX && niveaux[niveauActuel + 1].debloque)
        {
            if (selection == 1)
                setCouleur(240);
            printf("|             Niveau suivant                   |\n");
            setCouleur(7);
        }
        else
        {
            printf("|     Aucun niveau suivant disponible          |\n");
        }

        if (selection == 2)
            setCouleur(240);
        printf("|             Retour au menu principal         |\n");
        setCouleur(7);

        printf("+---------------------------------------------+\n");

        touche = _getch();
        if (touche == 0 || touche == 224)
            touche = _getch();

        selection = navigationMenu(selection, 1, 2, touche, NULL);

        if (touche == 13)
        {
            if (selection == 1 && niveauActuel + 1 < MAX_NIVEAUX && niveaux[niveauActuel + 1].debloque)
            {
                remove(fichierTemp); 
                free(fichierTemp);

                fichierTemp = creerNomFichierTemp(perso->nom);
                if (!copierFichier(niveaux[niveauActuel + 1].fichier, fichierTemp))
                {
                    printf("Erreur de chargement du niveau suivant\n");
                    Sleep(1500);
                    free(fichierTemp);
                    menuPrincipal(niveaux);
                    return;
                }
                niveauActuel++;
                mettreAJourCoordonnees(niveauActuel, &SPAWN_X, &SPAWN_Y, &MORT_Y);
                perso->positionX = SPAWN_X;
                perso->positionY = SPAWN_Y;
                jouer(fichierTemp, perso, niveaux);
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

void menuPrincipal(Niveau *niveaux)
{
    Personnage perso = {SPAWN_X, SPAWN_Y, "", 0, 0, 0, 1, 1, 1, 3};
    int selection = 1, touche;
    char *fichierTemp = NULL;
    int niveauActuel = 0;

    static Niveau niveauxLocaux[MAX_NIVEAUX];
    if (niveaux == NULL)
    {
        int niveauMaxDebloque = lireSauvegarde();
        initialiserNiveaux(niveauxLocaux, niveauMaxDebloque);
        niveaux = niveauxLocaux;
    }

    while (_kbhit())
        _getch();

    while (1)
    {
        afficherMenuPrincipal(selection, niveaux);

        touche = _getch();
        if (touche == 0 || touche == 224)
            touche = _getch();

        selection = navigationMenu(selection, 1, MAX_NIVEAUX + 3, touche, niveaux);

        if (touche == 13)
        {
            if (selection >= 1 && selection <= MAX_NIVEAUX && niveaux[selection - 1].debloque)
            {
                static int nomSaisi = 0;
                if (!nomSaisi || strlen(nomJoueurStocke) == 0)
                {
                    system("cls");
                    printf("Entrez votre nom: ");
                    scanf("%s", perso.nom);
                    strcpy(nomJoueurStocke, perso.nom);
                    nomSaisi = 1;
                }
                else
                {
                    strcpy(perso.nom, nomJoueurStocke);
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
                mettreAJourCoordonnees(niveauActuel, &SPAWN_X, &SPAWN_Y, &MORT_Y);
                perso.positionX = SPAWN_X;
                perso.positionY = SPAWN_Y;

                jouer(fichierTemp, &perso, niveaux);
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
                sauvegarderProgression(0); 
                
                initialiserNiveaux(niveaux, 0);
                
                printf("Scores et progression reinitialises !\n");
                Sleep(1500);
            }
            else if (selection == MAX_NIVEAUX + 3)
            {
                printf("Merci d'avoir joue !\n");
                exit(0);
            }
        }
    }
}