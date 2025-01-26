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

void deplacer_joueur(FILE *fichier, Personnage* perso, int largeur) {
    if (_kbhit()) {
        char touche = _getch();
        printf("%c", touche);
        int nouvelle_posx = perso->positionX;
        int nouvelle_posy = perso->positionY;

        switch (touche) {
            case 'q': 
                if (perso->positionX > 0) nouvelle_posx--;
                break;
            case 'd': 
                if (perso->positionX < largeur - 1) nouvelle_posx++;
                break;
            case 'z': 
                if (perso->positionY > 0) nouvelle_posy--;
                break;
            case 'e': 
                exit(0);
        }

        fseek(fichier, (perso->positionY) * largeur + perso->positionX, SEEK_SET);
        fputc(' ', fichier);

        fseek(fichier, nouvelle_posy * largeur + nouvelle_posx, SEEK_SET);
        fputc('M', fichier);

        perso->positionX = nouvelle_posx;
        perso->positionY = nouvelle_posy;

        rewind(fichier); 
    }
}

void jouer(const char *fichierTemp, Personnage* perso) {
    FILE *fichier = fopen(fichierTemp, "r+");

    fseek(fichier, (perso->positionY) * 100 + perso->positionX, SEEK_SET);
    fputc('M', fichier);

    int largeur = 100; 
    while (1) {
        system("cls"); 
        afficherPaysage(fichier, perso->positionX);
        deplacer_joueur(fichier, perso, largeur);
        Sleep(250);
    }
    
    fclose(fichier);
    
}