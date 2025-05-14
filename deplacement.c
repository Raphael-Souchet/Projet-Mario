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
        printf("@");
        break;
    case 'u':
        printf("u");
        break;
    case ']':
        printf("]");
        break;
    case '[':
        printf("[");
        break;
    case '|':
        printf("|");
        break;
    default:
        printf("%c", caractereActuel);
        break;
    }
}

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} BackgroundTexture;

static BackgroundTexture* globalBackground = NULL;

BackgroundTexture* loadBackgroundTexture(SDL_Renderer *renderer, const char *path)
{
    BackgroundTexture* bg = (BackgroundTexture*)malloc(sizeof(BackgroundTexture));
    if (bg == NULL) {
        printf("Erreur: Impossible d'allouer la mémoire pour la texture de fond\n");
        return NULL;
    }
    
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL) {
        printf("Erreur: Impossible de charger l'image %s: %s\n", path, IMG_GetError());
        free(bg);
        return NULL;
    }
    
    bg->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (bg->texture == NULL) {
        printf("Erreur: Impossible de créer la texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(bg);
        return NULL;
    }
    
    bg->width = surface->w;
    bg->height = surface->h;
    
    SDL_FreeSurface(surface);
    
    return bg;
}

void freeBackgroundTexture(BackgroundTexture* bg)
{
    if (bg != NULL) {
        if (bg->texture != NULL) {
            SDL_DestroyTexture(bg->texture);
        }
        free(bg);
    }
}

void afficherPaysageSDL(Carte *carte, int positionJoueur, SDL_Renderer *renderer)
{
    int largeurAffichage = 50;
    int demiLargeur = largeurAffichage / 2;
    
    int debutX = positionJoueur - demiLargeur;
    int finX = positionJoueur + demiLargeur;
    
    if (debutX < 0) {
        finX -= debutX; 
        debutX = 0;
    }
    
    if (finX > carte->largeur) {
        debutX -= (finX - carte->largeur);
        finX = carte->largeur;
    }
    
    if (debutX < 0) {
        debutX = 0;
    }
    
    
    if (globalBackground == NULL) {
        
        const char* bgPath = "C:\\Users\\duber\\cours L1\\programation\\mario_vscode\\Projet-Mario\\asset\\sprit\\Mondstadt Tileset Platform - Basic\\windrise-background.png";
        globalBackground = loadBackgroundTexture(renderer, bgPath);
        
        
        if (globalBackground == NULL) {
            printf("Impossible de charger l'image PNG. Tentative avec un fichier BMP...\n");
            bgPath = "C:\\Users\\duber\\cours L1\\programation\\mario_vscode\\Projet-Mario\\asset\\sprit\\Mondstadt Tileset Platform - Basic\\windrise-background.bmp";
            globalBackground = loadBackgroundTexture(renderer, bgPath);
        }
    }

    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    
    if (globalBackground != NULL && globalBackground->texture != NULL) {
        
        int backgroundOffset = (debutX * 3) % globalBackground->width;  
        
        
        SDL_Rect srcRect = {backgroundOffset, 0, globalBackground->width - backgroundOffset, globalBackground->height};
        SDL_Rect dstRect = {0, 0, largeurAffichage * TILE_SIZE, 20 * TILE_SIZE};
        
        
        SDL_RenderCopy(renderer, globalBackground->texture, &srcRect, &dstRect);
        
       
        if (backgroundOffset > 0) {
            srcRect.x = 0;
            srcRect.w = backgroundOffset;
            dstRect.x = (globalBackground->width - backgroundOffset) * (largeurAffichage * TILE_SIZE) / globalBackground->width;
            dstRect.w = largeurAffichage * TILE_SIZE - dstRect.x;
            SDL_RenderCopy(renderer, globalBackground->texture, &srcRect, &dstRect);
        }
    }

    
    for (int y = 0; y < carte->hauteur; y++)
    {
        for (int x = debutX; x < finX; x++)
        {
            if (x >= carte->largeur)
                continue;

            SDL_Rect tile = {
                (x - debutX) * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE};

            switch (carte->carte[y][x])
            {
            case 'w':
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            case 'c':
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            case 'M':
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            case 'Q':
                SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            case ']':
            case '[':
                SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            case 'u':
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            case '|':
                SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            case '*':
                SDL_SetRenderDrawColor(renderer, 241, 255, 65, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            case '!':
                SDL_SetRenderDrawColor(renderer, 25, 65, 199, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            default:
                
                break;
            }
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
    
    if (perso->etape_saut == 0 && perso->en_saut == 1)
    {
        perso->etape_saut = 1;
        return;
    }
    
    int hauteur_actuelle = hauteur_saut[perso->etape_saut - 1];
    
    int nouv_y = perso->positionY + hauteur_actuelle;
    
    if (nouv_y >= 0 && nouv_y < carte->hauteur)
    {
        if ((hauteur_actuelle < 0 && perso->peut_monter) || 
            (hauteur_actuelle > 0 && !perso->en_chute))
        {
            if (carte->carte[nouv_y][perso->positionX] == 'c')
            {
                perso->score++;
                carte->carte[nouv_y][perso->positionX] = ' ';
            }
            
            if (carte->carte[nouv_y][perso->positionX] != 'w')
            {
                effacer_position(carte, perso);
                perso->positionY = nouv_y;
                mettre_position(carte, perso);
            }
        }
    }
    
    perso->etape_saut++;
    
    if (perso->etape_saut > 8)
    {
        perso->en_saut = 0;
        perso->etape_saut = 0;
    }
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
    
    if (globalBackground != NULL)
    {
        freeBackgroundTexture(globalBackground);
        globalBackground = NULL;  
    }
    
    
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

    if (perso->en_chute && !perso->en_saut)
    {
        int new_y = perso->positionY + 1;
        if (new_y < carte->hauteur)
        {
            if (carte->carte[new_y][perso->positionX] == 'c')
            {
                perso->score++;
                carte->carte[new_y][perso->positionX] = ' ';
            }
            
            if (carte->carte[new_y][perso->positionX] != 'w')
            {
                effacer_position(carte, perso);
                perso->positionY = new_y;
                mettre_position(carte, perso);
                verifier_collision(carte, perso);
            }
        }
    }

    if (deplacement_x != 0)
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
                if (carte->carte[perso->positionY][new_x] != 'w')
                {
                    effacer_position(carte, perso);
                    perso->positionX = new_x;
                    mettre_position(carte, perso);
                    verifier_collision(carte, perso);
                }
            }
        }
    }

    if (perso->en_saut)
    {
        gerer_saut(carte, perso, deplacement_x);
    }
    else if ((GetAsyncKeyState('Z') & 0x8000) && !perso->en_chute)
    {
        perso->en_saut = 1;
        perso->etape_saut = 0;
        gerer_saut(carte, perso, deplacement_x);
    }
}

void jouer(const char *fichierTemp, Personnage *perso)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return;
    }

    
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
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

    if (window == NULL)
    {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return;
    }

    Carte *carte = chargerCarteEnMemoire(fichierTemp);
    if (carte == NULL)
    {
        printf("Erreur: Impossible de charger la carte %s\n", fichierTemp);
        nettoyerSDL(window, renderer);
        Sleep(1500);
        IMG_Quit();
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
    Tab_plante tab_plante = {NULL, 0};
    initialiserPlante(carte, &tab_plante);
    initialiser_gumbas(carte, &tab_gumba);

    cacherCurseur();
    
    int quit = 0;
    Uint32 lastTime = SDL_GetTicks();
    const int FPS = 22;
    const int frameDelay = 1500 / FPS;

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

        Uint32 currentTime = SDL_GetTicks();
        int deltaTime = currentTime - lastTime;

        if (deltaTime >= frameDelay)
        {
            lastTime = currentTime;

            bouger_gumba(carte, &tab_gumba);
            bougerPlante(carte, &tab_plante);

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            {
                while (_kbhit())
                    _getch();
                
                sauvegarderCarteVersFichier(carte, fichierTemp);

                nettoyerSDL(window, renderer);
                IMG_Quit();

                if (tab_gumba.gumbas != NULL)
                {
                    free(tab_gumba.gumbas);
                    tab_gumba.gumbas = NULL;
                }
                
                if (tab_plante.plantes != NULL)
                {
                    free(tab_plante.plantes);
                    tab_plante.plantes = NULL;
                }

                menuSauvegarde(perso, carte);
                return;
            }

            deplacer_joueur(carte, perso);

            gerer_collisions(carte, perso, &tab_gumba, &tab_plante);

            afficherPaysageSDL(carte, perso->positionX, renderer);
            SDL_RenderPresent(renderer);

            if (perso->positionY >= MORT_Y)
            {
                perso->vie--;
                sauvegarderCarteVersFichier(carte, fichierTemp);

                nettoyerSDL(window, renderer);
                IMG_Quit();

                if (tab_gumba.gumbas != NULL)
                {
                    free(tab_gumba.gumbas);
                    tab_gumba.gumbas = NULL;
                }
                
                if (tab_plante.plantes != NULL)
                {
                    free(tab_plante.plantes);
                    tab_plante.plantes = NULL;
                }

                libererCarte(carte);
                
                menu_mort(perso, fichierTemp);
                return;
            }
        }
    }

    nettoyerSDL(window, renderer);
    IMG_Quit();

    if (tab_gumba.gumbas != NULL)
    {
        free(tab_gumba.gumbas);
        tab_gumba.gumbas = NULL;
    }
    
    if (tab_plante.plantes != NULL)
    {
        free(tab_plante.plantes);
        tab_plante.plantes = NULL;
    }

    libererCarte(carte);
}