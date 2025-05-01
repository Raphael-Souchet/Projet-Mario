#include "projet.h"

void effacer_position(Carte *carte, Personnage *perso)
{
    carte->carte[perso->positionY][perso->positionX] = ' ';
}

void mettre_position(Carte *carte, Personnage *perso)
{
    carte->carte[perso->positionY][perso->positionX] = 'M';
}

void deplacerCurseur(int x, int y)
{
    COORD coord = {x, y};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, coord);
}

void caracterePaysage(char caractereActuel)
{
    switch (caractereActuel)
    {
    case 'w':
        printf("#");
        break;
    case 'c':
        printf("o");
        break;
    case 'M':
        printf("&");
        break;
    case 'Q':
        printf("Q");
        break;
    default:
        printf("%c", caractereActuel);
        break;
    }
}

void afficherPaysage(Carte *carte, int positionJoueur)
{
    int largeurAffichage = 82;
    int demiLargeur = largeurAffichage / 2;
    int debutX = positionJoueur - demiLargeur;
    int finX = positionJoueur + demiLargeur;

    if (debutX < 0)
    {
        debutX = 0;
        finX = largeurAffichage;
    }
    if (finX > carte->largeur)
    {
        finX = carte->largeur;
        debutX = finX - largeurAffichage;
        if (debutX < 0)
            debutX = 0;
    }

    char ligne[largeurAffichage + 1];
    ligne[largeurAffichage] = '\0';

    for (int y = 0; y < carte->hauteur; y++)
    {
        for (int x = debutX; x < finX; x++)
        {
            if (x < carte->largeur)
            {
                char c = carte->carte[y][x];
                switch (c)
                {
                case 'w':
                    ligne[x - debutX] = '#';
                    break;
                case 'c':
                    ligne[x - debutX] = 'o';
                    break;
                case 'M':
                    ligne[x - debutX] = '&';
                    break;
                case 'Q':
                    ligne[x - debutX] = '@';
                    break;
                default:
                    ligne[x - debutX] = c;
                    break;
                }
            }
            else
            {
                ligne[x - debutX] = ' ';
            }
        }
        printf("%s\n", ligne);
    }
}

void cacherCurseur()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    info.dwSize = 1;
    SetConsoleCursorInfo(console, &info);
}

void gerer_saut(Carte *carte, Personnage *perso, int direction)
{
    int hauteur_saut[8] = {-1, -1, -1, -1, 1, 1, 1, 1};

    for (int i = 0; i < 8; i++)
    {
        int nouv_x = perso->positionX + direction;
        int nouv_y = perso->positionY + hauteur_saut[i];

        if (nouv_x < 0 || nouv_x >= carte->largeur || nouv_y < 0 || nouv_y >= carte->hauteur)
        {
            continue;
        }

        if (carte->carte[nouv_y][nouv_x] == 'c')
        {
            perso->score++;
            carte->carte[nouv_y][nouv_x] = ' ';
        }

        char caractere_destination = carte->carte[nouv_y][nouv_x];
        char caractere_lateral = carte->carte[perso->positionY][nouv_x];

        if (nouv_x >= 0 && nouv_x < carte->largeur && nouv_y >= 0 &&
            caractere_destination != 'w' && caractere_lateral != 'w' && perso->peut_monter)
        {

            effacer_position(carte, perso);
            perso->positionX = nouv_x;
            perso->positionY = nouv_y;
            mettre_position(carte, perso);

            deplacerCurseur(0, 0);
            afficherPaysage(carte, perso->positionX);
            printf("Score: %d | Nom: %s | Vies: %d\n", perso->score, perso->nom, perso->vie);
            Sleep(85);
        }
    }

    perso->en_saut = 0;
}

void verifier_collision(Carte *carte, Personnage *perso)
{
    char caractere_dessous = (perso->positionY + 1 < carte->hauteur) ? carte->carte[perso->positionY + 1][perso->positionX] : 'w';

    if (caractere_dessous == 'w')
    {
        perso->en_chute = 0;
    }
    else
    {
        perso->en_chute = 1;
    }

    char caractere_dessus = (perso->positionY - 1 >= 0) ? carte->carte[perso->positionY - 1][perso->positionX] : 'w';

    if (caractere_dessus == 'w')
    {
        perso->peut_monter = 0;
    }
    else
    {
        perso->peut_monter = 1;
    }

    char caractere_devant = (perso->positionX + 1 < carte->largeur) ? carte->carte[perso->positionY][perso->positionX + 1] : 'w';

    if (caractere_devant == 'w' || perso->positionX == carte->largeur - 3)
    {
        perso->peut_avancer = 0;
    }
    else
    {
        perso->peut_avancer = 1;
    }

    char caractere_derriere = (perso->positionX - 1 >= 0) ? carte->carte[perso->positionY][perso->positionX - 1] : 'w';

    if (caractere_derriere == 'w' || perso->positionX == 0)
    {
        perso->peut_reculer = 0;
    }
    else
    {
        perso->peut_reculer = 1;
    }

    if (perso->positionX < 0)
        perso->positionX = 0;
    if (perso->positionX >= carte->largeur)
        perso->positionX = carte->largeur - 1;
    if (perso->positionY < 0)
        perso->positionY = 0;
}

void verifier_collision_gumba(Carte *carte, Gumba *gumba)
{
    char caractere_devant_dessous = (gumba->positionY + 1 < carte->hauteur && gumba->positionX + 1 < carte->largeur) ? carte->carte[gumba->positionY + 1][gumba->positionX + 1] : 'w';

    if (caractere_devant_dessous == ' ')
    {
        gumba->peut_tomber_devant = 1;
    }
    else
    {
        gumba->peut_tomber_devant = 0;
    }

    char caractere_derriere_dessous = (gumba->positionY + 1 < carte->hauteur && gumba->positionX - 1 >= 0) ? carte->carte[gumba->positionY + 1][gumba->positionX - 1] : 'w';

    if (caractere_derriere_dessous == ' ')
    {
        gumba->peut_tomber_derriere = 1;
    }
    else
    {
        gumba->peut_tomber_derriere = 0;
    }

    char caractere_devant = (gumba->positionX + 1 < carte->largeur) ? carte->carte[gumba->positionY][gumba->positionX + 1] : 'w';

    if (caractere_devant == 'w' || caractere_devant == ']' || caractere_devant == 'Q' || gumba->positionX == carte->largeur - 3)
    {
        gumba->peut_avancer = 0;
    }
    else
    {
        gumba->peut_avancer = 1;
    }
    char caractere_derriere = (gumba->positionX - 1 >= 0) ? carte->carte[gumba->positionY][gumba->positionX - 1] : 'w';

    if (caractere_derriere == 'w' || caractere_derriere == '[' || caractere_derriere == 'Q' || gumba->positionX == 0)
    {
        gumba->peut_reculer = 0;
    }
    else
    {
        gumba->peut_reculer = 1;
    }

    if (gumba->positionX < 0)
        gumba->positionX = 0;
    if (gumba->positionX >= carte->largeur)
        gumba->positionX = carte->largeur - 1;
    if (gumba->positionY < 0)
        gumba->positionY = 0;
}

void deplacer_joueur(Carte *carte, Personnage *perso)
{
    if (perso->positionY >= MORT_Y)
    {
        return;
    }

    int deplacement_x = 0;

    if (GetAsyncKeyState('D') & 0x8000)
        deplacement_x = 1;
    if (GetAsyncKeyState('Q') & 0x8000)
        deplacement_x = -1;

    verifier_collision(carte, perso);

    if (perso->en_chute)
    {
        int new_y = perso->positionY + 1;
        if (new_y < carte->hauteur)
        {
            if (carte->carte[new_y][perso->positionX] == 'c')
            {
                perso->score++;
                carte->carte[new_y][perso->positionX] = ' ';
            }
            effacer_position(carte, perso);
            perso->positionY = new_y;
            mettre_position(carte, perso);
            verifier_collision(carte, perso);
        }
    }

    if (deplacement_x != 0 && !perso->en_saut)
    {
        int new_x = perso->positionX + deplacement_x;
        if (new_x >= 0 && new_x < carte->largeur)
        {
            if (carte->carte[perso->positionY][new_x] == 'c')
            {
                perso->score++;
                carte->carte[perso->positionY][new_x] = ' ';
            }
            if ((deplacement_x > 0 && perso->peut_avancer) ||
                (deplacement_x < 0 && perso->peut_reculer))
            {
                effacer_position(carte, perso);
                perso->positionX = new_x;
                mettre_position(carte, perso);
            }
        }
    }

    if ((GetAsyncKeyState('Z') & 0x8000) && !perso->en_saut && !perso->en_chute)
    {
        perso->en_saut = 1;
        int direction = 0;

        if (deplacement_x > 0)
            direction = 1;
        else if (deplacement_x < 0)
            direction = -1;

        gerer_saut(carte, perso, direction);
    }
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        while (_kbhit())
            _getch();
        menuSauvegarde(perso, carte);
    }
}

void jouer(const char *fichierTemp, Personnage *perso)
{
    Carte *carte = chargerCarteEnMemoire(fichierTemp);
    if (carte == NULL)
    {
        printf("Erreur: Impossible de charger la carte %s\n", fichierTemp);
        Sleep(1500);
        return;
    }

    if (perso->positionX < 0 || perso->positionX >= carte->largeur ||
        perso->positionY < 0 || perso->positionY >= carte->hauteur)
    {
        perso->positionX = SPAWN_X;
        perso->positionY = SPAWN_Y;
    }

    carte->carte[perso->positionY][perso->positionX] = 'M';

    Tab_gumba tab_gumba = {NULL, 0};
    initialiser_gumbas(carte, &tab_gumba);

    cacherCurseur();

    while (1)
    {
        Sleep(60);
        deplacerCurseur(0, 0);
        bouger_gumba(carte, &tab_gumba);
        afficherPaysage(carte, perso->positionX);
        printf("Score: %d | Nom: %s | Vies: %d\n", perso->score, perso->nom, perso->vie);
        deplacer_joueur(carte, perso);

        if (perso->positionY >= MORT_Y)
        {
            perso->vie--;
            sauvegarderCarteVersFichier(carte, fichierTemp);

            if (tab_gumba.gumbas != NULL)
            {
                free(tab_gumba.gumbas);
                tab_gumba.gumbas = NULL;
            }

            libererCarte(carte);
            menu_mort(perso, fichierTemp);
            return;
        }
    }

    if (tab_gumba.gumbas != NULL)
    {
        free(tab_gumba.gumbas);
        tab_gumba.gumbas = NULL;
    }

    libererCarte(carte);
}