#include "projet.h"

int LARGEUR_MAP = 163;
int SPAWN_X = 21;
int SPAWN_Y = 15;   
int SPAWN_MORT_X = 21;
int SPAWN_MORT_Y = 14;
int MORT_Y = 19;
char nomJoueurStocke[100]= "";
Niveau niveaux[MAX_NIVEAUX];
int niveauActuel = 0; 
int niveauMaxDebloque = 0;

GameTextures* gameTextures = NULL;
Animation* carnivoreAnimation = NULL;
PlayerAnimations* playerAnimations = NULL;
Animation* flagAnimation = NULL;
BackgroundTexture* globalBackground = NULL;
TTF_Font* scoreFont = NULL;
SDL_Color scoreColor = {255, 255, 255, 255};
Tab_piece tab_pieces = {NULL, 0, 0};
Animation* coinAnimation = NULL;
Tab_starcoins tab_starcoins = {NULL, 0, 0};
Animation* starCoinAnimation = NULL;
SDL_Texture* heartTexture = NULL;

int main(int argc, char *argv[])
{
    int niveauMaxDebloque = lireSauvegarde();
    const char *fichierOriginal = "Mario.txt";
    
    initialiserNiveaux(niveaux, niveauMaxDebloque);
    menuPrincipal(niveaux);
    return 0;
}


void jouer(const char *fichierTemp, Personnage *perso, Niveau *niveaux)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        printf("Erreur IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return;
    }

    if (!initGameAudio())
    {
        printf("Avertissement: L'audio n'a pas pu être initialisé. Le jeu continuera sans son.\n");
    }

    int largeurAffichage = 50;
    SDL_Window *window = SDL_CreateWindow(
        "Super Mario",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        largeurAffichage * TILE_SIZE,
        25 * TILE_SIZE,
        0);

    if (window == NULL)
    {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        cleanupAudio();
        IMG_Quit();
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        cleanupAudio();
        IMG_Quit();
        SDL_Quit();
        return;
    }

    scoreFont = initFont();
    if (scoreFont == NULL)
    {
        printf("Avertissement: Impossible de charger la police. Le score ne sera pas affiché.\n");
    }

    Carte *carte = chargerCarteEnMemoire(fichierTemp);
    if (carte == NULL)
    {
        printf("Erreur: Impossible de charger la carte %s\n", fichierTemp);
        nettoyerSDL(window, renderer);
        cleanupAudio();
        SDL_Quit();
        return;
    }

    if (perso->positionX < 0 || perso->positionX >= carte->largeur ||
        perso->positionY < 0 || perso->positionY >= carte->hauteur)
    {
        perso->positionX = SPAWN_X;
        perso->positionY = SPAWN_Y;
    }

    perso->vitesse_x = 0.0f;
    carte->carte[perso->positionY][perso->positionX] = 'M';

    Tab_gumba tab_gumba = {NULL, 0};
    Tab_plante tab_plante = {NULL, 0};
    initialiser_gumbas(carte, &tab_gumba);
    initialiserPlante(carte, &tab_plante);

    initialiser_pieces(carte, &tab_pieces);
    initialiser_starcoins(carte, &tab_starcoins);

    cacherCurseur();

    int quit = 0;
    Uint32 lastTime = SDL_GetTicks();
    const int FPS = 22;
    const int frameDelay = 1500 / FPS;

    int playerMoving = 0;

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

                int choixMenu = menuPauseEnJeu();

                if (choixMenu == 2)
                {
                    stopBackgroundMusic();
                    cleanupAudio();

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

                    return;
                }
            }

            deplacer_joueur(carte, perso, &playerMoving, &tab_gumba, &tab_plante, fichierTemp, window, renderer, niveauActuel);

            check_collect_piece(carte, &tab_pieces, perso);
            check_collect_starcoin(carte, &tab_starcoins, perso);

            gerer_collisions(carte, perso, &tab_gumba, &tab_plante);

            afficherPaysageSDL(carte, perso->positionX, renderer, playerMoving, perso);
            SDL_RenderPresent(renderer);

            if (perso->positionY >= MORT_Y)
            {
                playSoundEffect("asset/sound/death.wav", 64);
                Sleep(2500);
                perso->vie--;
                sauvegarderCarteVersFichier(carte, fichierTemp);

                stopBackgroundMusic();
                cleanupAudio();

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

                menu_mort(perso, fichierTemp, niveauActuel);
                return;
            }
        }
    }

    stopBackgroundMusic();
    cleanupAudio();

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