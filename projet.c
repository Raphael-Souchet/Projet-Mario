#include "projet.h"

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

int menu() {
    int choix;
    system("cls");
    printf("+---------------------------------------------+");
    printf("\n|     Bienvenue sur le meilleur jeu Mario     |\n");
    printf("|                                             |\n");  
    printf("|                   - Menu -                  |\n");  
    printf("|                                             |\n");  
    printf("|                  1. Jouer                   |\n");  
    printf("|                  2. Score                   |\n");  
    printf("|                  3. Quitter                 |\n"); 
    printf("+---------------------------------------------+\n");
    printf("Choisissez une option : ");
    scanf("%d",&choix);

    if (choix != 1 && choix != 2 && choix != 3) {
        printf("Choix invalide, veuillez reessayer\n");
        Sleep(1000);
        menu();
    }
    else {
        system("cls");
        return choix;
    }
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

void afficherPaysage(FILE *fichier, int *positionJoueur) {
    char ligne[1024];

    int debutLecture = *positionJoueur - 21;
    int finLecture = *positionJoueur + 21;

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

void deplacer_joueur(FILE *fichier, int *posx, int *posy, int largeur) {
    if (_kbhit()) {
        char touche = _getch();
        int nouvelle_posx = *posx;
        int nouvelle_posy = *posy;

        switch (touche) {
            case 'q': 
                if (*posx > 0) nouvelle_posx--;
                break;
            case 'd': 
                if (*posx < largeur - 1) nouvelle_posx++;
                break;
            case 'z': 
                if (*posy > 0) nouvelle_posy--;
                break;
            case 'e': 
                exit(0);
        }

        fseek(fichier, (*posy) * largeur + *posx, SEEK_SET);
        fputc(' ', fichier);

        fseek(fichier, nouvelle_posy * largeur + nouvelle_posx, SEEK_SET);
        fputc('M', fichier);

        *posx = nouvelle_posx;
        *posy = nouvelle_posy;

        rewind(fichier); 
    }
}

void jouer(const char *fichierTemp, int posx, int posy) {
    FILE *fichier = fopen(fichierTemp, "r+");

    fseek(fichier, (posy) * 100 + posx, SEEK_SET);
    fputc('M', fichier);

    int largeur = 100; 
    while (1) {
        system("cls"); 
        afficherPaysage(fichier, &posx);
        deplacer_joueur(fichier, &posx, &posy, largeur);
        Sleep(250);
    }
    
    fclose(fichier);
    
}