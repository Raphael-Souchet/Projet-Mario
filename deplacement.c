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

void afficherPaysageSDL(Carte *carte, int positionJoueur, SDL_Renderer *renderer) {
    int largeurAffichage = 50;
    int demiLargeur = largeurAffichage / 2;
    int debutX = positionJoueur - demiLargeur;
    int finX = positionJoueur + demiLargeur;

    if (debutX < 0) debutX = 0;
    if (finX > carte->largeur) finX = carte->largeur;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    for (int y = 0; y < carte->hauteur; y++) {
        for (int x = debutX; x < finX; x++) {
            if (x >= carte->largeur) continue;

            SDL_Rect tile = {
                (x - debutX) * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            switch(carte->carte[y][x]) {
                case 'w': 
                    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); 
                    break;
                case 'c': 
                    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                    break;
                case 'M':
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    break;
                case 'Q': 
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); 
                    break;
            }
            
            SDL_RenderFillRect(renderer, &tile);
        }
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

    SDL_Renderer *renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));

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

            if (renderer != NULL)
            {
                afficherPaysageSDL(carte, perso->positionX, renderer);
                SDL_RenderPresent(renderer);
            }

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

void nettoyerSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
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
}

void jouer(const char *fichierTemp, Personnage *perso)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return;
    }

    int largeurAffichage = 50; 
    SDL_Window *window = SDL_CreateWindow(
        "Super Mario",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        largeurAffichage * TILE_SIZE,
        20 * TILE_SIZE,
        0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Carte *carte = chargerCarteEnMemoire(fichierTemp);
    if (carte == NULL)
    {
        printf("Erreur: Impossible de charger la carte %s\n", fichierTemp);
        nettoyerSDL(window, renderer);
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
    int quit = 0;

    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        Sleep(60);
        bouger_gumba(carte, &tab_gumba);

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            while (_kbhit())
                _getch();
            sauvegarderCarteVersFichier(carte, fichierTemp);

            nettoyerSDL(window, renderer);

            if (tab_gumba.gumbas != NULL)
            {
                free(tab_gumba.gumbas);
                tab_gumba.gumbas = NULL;
            }

            menuSauvegarde(perso, carte);
            return;
        }

        deplacer_joueur(carte, perso);

        afficherPaysageSDL(carte, perso->positionX, renderer);
        SDL_RenderPresent(renderer);

        if (perso->positionY >= MORT_Y)
        {
            perso->vie--;
            sauvegarderCarteVersFichier(carte, fichierTemp);

            nettoyerSDL(window, renderer);

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

    nettoyerSDL(window, renderer);

    if (tab_gumba.gumbas != NULL)
    {
        free(tab_gumba.gumbas);
        tab_gumba.gumbas = NULL;
    }

    libererCarte(carte);
}