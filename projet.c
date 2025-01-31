#include "projet.h"

void effacer_position(FILE *fichier, Personnage *perso, int largeur) {
    fseek(fichier, (perso->positionY) * largeur + perso->positionX, SEEK_SET);
    fputc(' ', fichier);
}

void mettre_position(FILE *fichier, Personnage *perso, int largeur) {
    fseek(fichier, (perso->positionY) * largeur + perso->positionX, SEEK_SET);
    fputc('M', fichier);
}

void gerer_saut(FILE *fichier, Personnage *perso, int largeur, int direction) {
    int positions_saut[4][2];
    
    if (direction == 0) {  
        int saut_vertical[4][2] = {
            {0, -1},  
            {0, -1}, 
            {0, 1},  
            {0, 1}   
        };
        memcpy(positions_saut, saut_vertical, sizeof(positions_saut));
    }
    else if (direction == 1) { 
        int saut_droite[4][2] = {
            {1, -1}, 
            {1, -1}, 
            {1, 1},  
            {1, 1}   
        };
        memcpy(positions_saut, saut_droite, sizeof(positions_saut));
    }
    else { 
        int saut_gauche[4][2] = {
            {-1, -1}, 
            {-1, -1}, 
            {-1, 1},  
            {-1, 1}  
        };
        memcpy(positions_saut, saut_gauche, sizeof(positions_saut));
    }
    
    for (int i = 0; i < 4; i++) {
        int ancien_x = perso->positionX;
        int ancien_y = perso->positionY;
        
        int nouveau_x = perso->positionX + positions_saut[i][0];
        int nouveau_y = perso->positionY + positions_saut[i][1];
        
        if (nouveau_x >= 0 && nouveau_x < largeur && nouveau_y >= 0) {
            effacer_position(fichier, perso, largeur);
            
            perso->positionX = nouveau_x;
            perso->positionY = nouveau_y;
            
            mettre_position(fichier, perso, largeur);
            
            system("cls");
            afficherPaysage(fichier, perso->positionX);
            Sleep(100);
        } else {
            perso->positionX = ancien_x;
            perso->positionY = ancien_y;
        }
    }
    
    perso->en_saut = 0;
}

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
        int direction = 0;  
        char touche = _getch();
        
        if (_kbhit()) {
            char deuxieme_touche = _getch();
            
            if ((touche == 'z' && (deuxieme_touche == 'd' || deuxieme_touche == 'q')) ||
                ((touche == 'd' || touche == 'q') && deuxieme_touche == 'z')) {
                if (touche == 'd' || deuxieme_touche == 'd') {
                    direction = 1;  
                } else {
                    direction = -1; 
                }
                
                if (!perso->en_saut) {
                    perso->en_saut = 1;
                    gerer_saut(fichier, perso, largeur, direction);
                    return;
                }
            }
        }
        
        if (!perso->en_saut) {
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
                    perso->en_saut = 1;
                    gerer_saut(fichier, perso, largeur, 0);  
                    return;
                case 'e': 
                    exit(0);
            }

            if (nouvelle_posx != perso->positionX || nouvelle_posy != perso->positionY) {
                effacer_position(fichier, perso, largeur);
                perso->positionX = nouvelle_posx;
                perso->positionY = nouvelle_posy;
                mettre_position(fichier, perso, largeur);
            }
        }

        rewind(fichier);
        
        while (_kbhit()) {
            _getch();
        }
    }
}

void jouer(const char *fichierTemp, Personnage* perso) {
    FILE *fichier = fopen(fichierTemp, "r+");

    fseek(fichier, (perso->positionY) * 100 + perso->positionX, SEEK_SET);
    fputc('M', fichier);

    int largeur = 100; 
    while (1) {
        system("cls"); 
        deplacer_joueur(fichier, perso, largeur);
        afficherPaysage(fichier, perso->positionX);
        Sleep(200);
    }
    
    fclose(fichier);
    
}