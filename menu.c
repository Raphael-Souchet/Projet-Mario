#include "projet.h"

void setCouleur(int couleur) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, couleur);
}

void afficherOption(int numero, const char *texte, int selection_actuelle) {
    if (selection_actuelle == numero) {
        setCouleur(240 );
        printf("|              %d. %-30s |\n", numero, texte);
        setCouleur(7);
    } else {
        printf("|              %d. %-30s |\n", numero, texte);
    }
}

void afficherMenuSauvegarde(int selection) {
    system("cls");
    printf("+---------------------------------------------+\n");
    printf("|               Menu Sauvegarde               |\n");
    printf("|                                             |\n");
    
    setCouleur(7);
    if (selection == 1) setCouleur(240 );
    printf("|             Sauvegarder la partie           |\n");
    setCouleur(7);
    
    if (selection == 2) setCouleur(240);
    printf("|             Retour au jeu                   |\n");
    setCouleur(7);
    
    if (selection == 3) setCouleur(240 );
    printf("|             Menu Principal                  |\n");
    setCouleur(7);
    
    printf("+---------------------------------------------+\n");
}

int navigationMenu(int selection, int min, int max, int touche) {
    if (touche == 72 || touche == 122 || touche == 90 ) {
        return (selection > min) ? selection - 1 : max; 
    }
    else if (touche == 80 || touche == 115  || touche == 83) {
        return (selection < max) ? selection + 1 : min;
    }
    
    return selection;
}

void menuSauvegarde(Personnage *perso, Carte *carte) {
    char *fichierTemp = creerNomFichierTemp(perso->nom);
    int selection = 1;
    int touche;
    
    while (_kbhit()) _getch();
    
    while (1) {
        afficherMenuSauvegarde(selection);
        
        touche = _getch();
        
        if (touche == 0 || touche == 224) {
            touche = _getch();
        }
        
        selection = navigationMenu(selection, 1, 3, touche);
        
        if (touche == 13) {
            switch (selection) {
                case 1:
                    sauvegarderPartie(perso, carte, fichierTemp);
                    break;
                case 2:
                    free(fichierTemp);
                    jouer(fichierTemp, perso);
                    return;
                case 3:
                    sauvegarderCarteVersFichier(carte, fichierTemp);
                    libererCarte(carte);
                    free(fichierTemp);
                    menuPrincipal("Mario.txt");
                    return;
            }
        }
    }
}

void afficherMenuPrincipal(int selection) {
    system("cls");
    printf("+---------------------------------------------+\n");
    printf("|     Bienvenue sur le meilleur jeu Mario     |\n");
    printf("|                                             |\n");
    printf("|                   - Menu -                  |\n");
    printf("|                                             |\n");
    
    if (selection == 1) setCouleur(240 );
    printf("|                 Nouvelle Partie             |\n");
    setCouleur(7);
    
    if (selection == 2) setCouleur(240 );
    printf("|                 Charger Partie              |\n");
    setCouleur(7);
    
    if (selection == 3) setCouleur(240 );
    printf("|                 Score                       |\n");
    setCouleur(7);
    
    if (selection == 4) setCouleur(240 );
    printf("|                 Reset les scores            |\n");
    setCouleur(7);
    
    if (selection == 5) setCouleur(240 );
    printf("|                 Quitter                     |\n");
    setCouleur(7);
    
    printf("+---------------------------------------------+\n");
}

void menuPrincipal(const char *fichierOriginal) {
    Personnage perso = {SPAWN_X, SPAWN_Y, "", 0, 0, 0, 1, 1, 1, 3};
    int selection = 1; 
    int touche;
    char *fichierTemp = NULL;
    
    while (_kbhit()) _getch();
    
    while (1) {
        afficherMenuPrincipal(selection);
        
        touche = _getch();
        
        if (touche == 0 || touche == 224) {
            touche = _getch();
        }
        
        selection = navigationMenu(selection, 1, 5, touche);
        
        if (touche == 13) {
            switch (selection) {
                case 1:
                    system("cls");
                    printf("Entrez votre nom: ");
                    scanf("%s", perso.nom);

                    FILE *fichier_verif = fopen("sauvegarde.txt", "r");
                    int nom_existe = 0;

                    if (fichier_verif != NULL) {
                        char ligne[100], nom_check[100];
                        while (fgets(ligne, sizeof(ligne), fichier_verif)) {
                            if (sscanf(ligne, "Nom:%s", nom_check) == 1) {
                                if (strcmp(nom_check, perso.nom) == 0) {
                                    nom_existe = 1;
                                    break;
                                }
                            }
                        }
                        fclose(fichier_verif);
                    }

                    if (nom_existe) {
                        printf("Un joueur avec ce nom existe deja. Voulez-vous continuer? (o/n): ");
                        char choix;
                        scanf(" %c", &choix);
                        if (choix != 'o' && choix != 'O') {
                            continue; 
                        }
                    }

                    fichierTemp = creerNomFichierTemp(perso.nom);
                    remove(fichierTemp);

                    if (!copierFichier(fichierOriginal, fichierTemp)) {
                        printf("Erreur lors de la copie du fichier original\n");
                        Sleep(1500);
                        free(fichierTemp);
                        continue; 
                    }

                    jouer(fichierTemp, &perso);
                    free(fichierTemp);
                    return;
                    
                case 2: 
                    system("cls");
                    if (chargerPartie(&perso)) {
                        fichierTemp = creerNomFichierTemp(perso.nom);

                        FILE *test = fopen(fichierTemp, "r");
                        if (test) {
                            fclose(test);
                            jouer(fichierTemp, &perso);
                        } else {
                            printf("Erreur: Fichier de jeu temporaire manquant !\n");
                            Sleep(1500);
                        }
                        free(fichierTemp);
                        Sleep(1500);
                        system("cls");
                        return;
                    }
                    break;
                    
                case 3: 
                    system("cls");
                    afficherScores();
                    printf("\nAppuyez sur une touche pour revenir au menu...");
                    _getch();
                    system("cls");
                    break;
                    
                case 4: 
                    system("cls");
                    resetScores();
                    printf("Scores reinitialises avec succes!\n");
                    Sleep(1500);
                    system("cls");
                    break;
                    
                case 5: 
                    system("cls");
                    printf("Adios !\n");
                    exit(0);
            }
        }
    }
}

void afficherMenuMort(Personnage *perso, int selection) {
    system("cls");
    if (perso->vie > 0) {
        printf("+---------------------------------------------+\n");
        printf("|                 VOUS ETES MORT!             |\n");
        printf("|               Vies restantes : %d            |\n", perso->vie);
        printf("|                                             |\n");
        
        if (selection == 1) setCouleur(240  );
        printf("|                  Recommencer                |\n");
        setCouleur(7);
        
        if (selection == 2) setCouleur(240 );
        printf("|                 Menu Principal              |\n");
        setCouleur(7);
        
        printf("+---------------------------------------------+\n");
    } else {
        printf("+---------------------------------------------+\n");
        printf("|                 GAME OVER !                 |\n");
        printf("|                                             |\n");
        printf("|         Appuyez sur une touche pour         |\n");
        printf("|         retourner au menu principal         |\n");
        printf("+---------------------------------------------+\n");
    }
}

void menu_mort(Personnage *perso, const char *fichierTemp) {
    if (perso->vie > 0) {
        int selection = 1; 
        int touche;
        
        while (_kbhit()) _getch();
        
        while (1) {
            afficherMenuMort(perso, selection);
            
            touche = _getch();
            
            if (touche == 0 || touche == 224) {
                touche = _getch();
            }
            
            selection = navigationMenu(selection, 1, 2, touche);
            
            if (touche == 13) {
                if (selection == 1) {
                    remove(fichierTemp);
                    if (!copierFichier("Mario.txt", fichierTemp)) {
                        printf("Erreur lors de la reinitialisation du niveau !\n");
                        Sleep(1500);
                        menuPrincipal("Mario.txt");
                        return;
                    }
                    perso->positionX = SPAWN_MORT_X;
                    perso->positionY = SPAWN_Y;
                    perso->score = 0;
                    jouer(fichierTemp, perso);
                    return;
                } else {
                    menuPrincipal("Mario.txt");
                    return;
                }
            }
        }
    } else {
        afficherMenuMort(perso, 0);
        while (_kbhit()) _getch();
        _getch();
        menuPrincipal("Mario.txt");
    }
}