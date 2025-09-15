#include "projet.h"

int LARGEUR_MAP = 163;
int SPAWN_X = 21;
int SPAWN_Y = 15;
int MORT_Y = 23;
char nomJoueurStocke[100] = "";
Niveau niveaux[MAX_NIVEAUX];
int niveauActuel = 0;
int niveauMaxDebloque = 0;
int menuMusicPlaying = 0;

GameTextures *gameTextures = NULL;
Animation *carnivoreAnimation = NULL;
PlayerAnimations *playerAnimations = NULL;
Animation *flagAnimation = NULL;
BackgroundTexture *globalBackground = NULL;
TTF_Font *scoreFont = NULL;
SDL_Color scoreColor = {255, 255, 255, 255};
Tab_piece tab_pieces = {NULL, 0, 0};
Animation *coinAnimation = NULL;
Tab_starcoins tab_starcoins = {NULL, 0, 0};
Animation *starCoinAnimation = NULL;
SDL_Texture *heartTexture = NULL;
Animation *slimeAnimation = NULL;

int main(int argc, char *argv[])
{
    SauvegardeInfo saves[100];
    int nbSaves = lireSauvegardesExistant(saves, 100);
    char nomJoueur[100] = "";

    if (nbSaves > 0)
    {
        printf("Voulez-vous reprendre une partie existante ? (O/N) ");
        char choix;
        scanf(" %c", &choix);

        if (toupper(choix) == 'O')
        {
            system("cls");
            printf("+---------------------------------------------+\n");
            printf("|            PARTIES SAUVEGARDEES             |\n");
            printf("+---------------------------------------------+\n");

            for (int i = 0; i < nbSaves; i++)
            {
                printf("| %2d. %-39s |\n", i + 1, saves[i].nom);
            }
            printf("+---------------------------------------------+\n");

            if (nbSaves == 1)
            {
                strcpy(nomJoueur, saves[0].nom);
                printf("Une seule sauvegarde trouvee: %s\n", nomJoueur);
                printf("Chargement automatique...\n");
                Sleep(1500);
            }
            else
            {
                int choixJoueur = 0;
                do
                {
                    printf("Entrez le numero du joueur (1-%d) : ", nbSaves);
                    scanf("%d", &choixJoueur);
                } while (choixJoueur < 1 || choixJoueur > nbSaves);

                strcpy(nomJoueur, saves[choixJoueur - 1].nom);
                printf("Vous avez choisi: %s\n", nomJoueur);
                Sleep(1000);
            }

            int found = 0;
            for (int i = 0; i < nbSaves; i++)
            {
                if (strcmp(saves[i].nom, nomJoueur) == 0)
                {
                    niveauMaxDebloque = saves[i].niveauMaxDebloque;
                    strcpy(nomJoueurStocke, nomJoueur);
                    SPAWN_X = saves[i].positionX;
                    SPAWN_Y = saves[i].positionY;
                    found = 1;
                    break;
                }
            }

            if (!found)
            {
                printf("Aucune sauvegarde trouvee pour %s. Nouvelle partie.\n", nomJoueur);
                niveauMaxDebloque = 0;
                nomJoueurStocke[0] = '\0';
                Sleep(1500);
            }
        }
        else
        {

            niveauMaxDebloque = 0;
            nomJoueurStocke[0] = '\0';
        }
    }
    else
    {

        niveauMaxDebloque = 0;
        nomJoueurStocke[0] = '\0';
    }

    initialiserNiveaux(niveaux, niveauMaxDebloque);
    menuPrincipal(niveaux);
    return 0;
}

void jouer(const char *fichierTemp, Personnage *perso)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return;
    }

    reinitialiserAnimations();

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        printf("Erreur IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return;
    }
    char *musicpath = NULL;

    if (initGameAudio())
    {
        stopBackgroundMusic();

        char *musicpath = NULL;
        switch (niveauActuel)
        {
        case 0:
            musicpath = "asset/music/overworld.mp3";
            break;
        case 1:
            musicpath = "asset/music/beach.mp3";
            break;
        case 2:
            musicpath = "asset/music/sky.mp3";
            break;
        case 3:
            musicpath = "asset/music/tower.mp3";
            break;
        case 4:
            musicpath = "asset/music/swow.mp3";
            break;
        default:
            musicpath = "asset/music/overworld.mp3";
        }

        if (loadBackgroundMusic(musicpath))
        {
            setMusicVolume(96);
            playBackgroundMusic(-1);
            menuMusicPlaying = 0; 
        }
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
        cleanupAudio();
        IMG_Quit();
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_DestroyWindow(window);
        cleanupAudio();
        IMG_Quit();
        SDL_Quit();
        return;
    }
    perso->score = 0;
    scoreFont = initFont();

    Carte *carte = chargerCarteEnMemoire(fichierTemp);
    if (carte == NULL)
    {
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
    
    mettreAJourCoordonnees(&SPAWN_X, &SPAWN_Y, &MORT_Y);

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
                    menuMusicPlaying = 0;
                    nettoyerSDL(window, renderer);
                    IMG_Quit();
                    libererCarte(carte);
                    menuPrincipal(niveaux);
                    return;
                }
            }

            deplacer_joueur(carte, perso, &playerMoving, &tab_gumba, &tab_plante, fichierTemp, window, renderer);

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

                menu_mort(perso, fichierTemp);
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