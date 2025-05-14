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

BackgroundTexture *loadBackgroundTexture(SDL_Renderer *renderer, const char *path)
{
    BackgroundTexture *bg = (BackgroundTexture *)malloc(sizeof(BackgroundTexture));
    if (bg == NULL)
    {
        printf("Erreur: Impossible d'allouer la mémoire pour la texture de fond\n");
        return NULL;
    }

    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL)
    {
        printf("Erreur: Impossible de charger l'image %s: %s\n", path, IMG_GetError());
        free(bg);
        return NULL;
    }

    bg->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (bg->texture == NULL)
    {
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

void freeBackgroundTexture(BackgroundTexture *bg)
{
    if (bg != NULL)
    {
        if (bg->texture != NULL)
        {
            SDL_DestroyTexture(bg->texture);
        }
        free(bg);
    }
}

GameTextures *loadGameTextures(SDL_Renderer *renderer)
{
    GameTextures *textures = (GameTextures *)malloc(sizeof(GameTextures));
    if (textures == NULL)
    {
        printf("Erreur: Impossible d'allouer la mémoire pour les textures du jeu\n");
        return NULL;
    }

    textures->brick = NULL;

    SDL_Surface *surface = IMG_Load("asset/sprit/tiles/tile_0004.png");
    if (surface == NULL)
    {
        printf("Erreur: Impossible de charger l'image tile_0004.png: %s\n", IMG_GetError());
        surface = IMG_Load("asset/tiles/tile_0004.png");
        if (surface == NULL)
        {
            printf("Erreur: Impossible de charger l'image alternative: %s\n", IMG_GetError());
            free(textures);
            return NULL;
        }
    }

    textures->brick = SDL_CreateTextureFromSurface(renderer, surface);
    if (textures->brick == NULL)
    {
        printf("Erreur: Impossible de créer la texture de brique: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(textures);
        return NULL;
    }

    SDL_FreeSurface(surface);

    return textures;
}

void freeGameTextures(GameTextures *textures)
{
    if (textures != NULL)
    {
        if (textures->brick != NULL)
        {
            SDL_DestroyTexture(textures->brick);
        }
        free(textures);
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
Animation *loadAnimation(SDL_Renderer *renderer, const char *path, int frameCount, int frameWidth, int frameHeight, Uint32 frameDuration)
{
    Animation *animation = (Animation *)malloc(sizeof(Animation));
    if (animation == NULL)
    {
        printf("Erreur: Impossible d'allouer la mémoire pour l'animation\n");
        return NULL;
    }

    animation->texture = NULL;
    animation->frameCount = frameCount;
    animation->currentFrame = 0;
    animation->frameWidth = frameWidth;
    animation->frameHeight = frameHeight;
    animation->frameDuration = frameDuration;
    animation->lastFrameTime = SDL_GetTicks();

    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL)
    {
        printf("Erreur: Impossible de charger l'image %s: %s\n", path, IMG_GetError());
        free(animation);
        return NULL;
    }

    animation->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (animation->texture == NULL)
    {
        printf("Erreur: Impossible de créer la texture d'animation: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(animation);
        return NULL;
    }

    SDL_FreeSurface(surface);

    return animation;
}
void loadCoinAnimations(SDL_Renderer *renderer)
{
    // Chargement des animations de pièces si elles ne sont pas déjà chargées
    if (coinAnimation == NULL)
    {
        // Pour les pièces normales (c)
        const char *coinPaths[] = {
            "asset/brackeys_platformer_assets/sprites/coin.png",
            "asset/sprites/coin.png",
            "asset/sprit/sprites/coin.png",
            "asset/coin.png"};

        // Essayer différents chemins jusqu'à ce qu'un fonctionne
        for (int i = 0; i < 4 && coinAnimation == NULL; i++)
        {
            coinAnimation = loadAnimation(renderer, coinPaths[i], 6, 16, 16, 100);
            if (coinAnimation != NULL)
            {
                printf("Animation des pièces chargée avec succès: %s\n", coinPaths[i]);
            }
        }

        // Si aucun chemin ne fonctionne, informer l'utilisateur
        if (coinAnimation == NULL)
        {
            printf("Échec du chargement de l'animation des pièces. Utilisation de la représentation par défaut.\n");
        }
    }

    if (starCoinAnimation == NULL)
    {
        // Pour les pièces étoiles (*)
        const char *starCoinPaths[] = {
            "asset/brackeys_platformer_assets/sprites/starcoin.png",
            "asset/sprites/starcoin.png",
            "asset/sprit/sprites/starcoin.png",
            "asset/starcoin.png",
            // Ajouter le chemin de la nouvelle image si différent
            "asset/starcoin.png"};

        // Essayer différents chemins jusqu'à ce qu'un fonctionne
        for (int i = 0; i < 5 && starCoinAnimation == NULL; i++)
        {
            // Utiliser 6 frames de 16x16 pixels, avec un intervalle de 100ms
            starCoinAnimation = loadAnimation(renderer, starCoinPaths[i], 6, 16, 16, 100);
            if (starCoinAnimation != NULL)
            {
                printf("Animation des pièces étoiles chargée avec succès: %s\n", starCoinPaths[i]);
            }
        }

        // Si l'image ne peut pas être chargée via les chemins standards, essayer de charger directement l'image fournie
        if (starCoinAnimation == NULL)
        {
            // On peut charger directement depuis le chemin où vous avez placé l'image
            starCoinAnimation = loadAnimation(renderer, "asset/starcoin.png", 6, 16, 16, 100);
            if (starCoinAnimation != NULL)
            {
                printf("Animation des pièces étoiles chargée depuis l'image de rotation!\n");
            }
        }
  
    }
}
PlayerAnimations *loadPlayerAnimations(SDL_Renderer *renderer)
{
    PlayerAnimations *animations = (PlayerAnimations *)malloc(sizeof(PlayerAnimations));
    if (animations == NULL)
    {
        printf("Erreur: Impossible d'allouer la mémoire pour les animations du joueur\n");
        return NULL;
    }

    animations->idle = NULL;
    animations->idle_left = NULL;
    animations->run = NULL;
    animations->run_left = NULL;
    animations->facingRight = 1;

    const char *paths[][4] = {
        {"asset/santa_sheets/idle_sheet.png",
         "asset/santa_sheets/run_sheet.png",
         "asset/santa_sheets/idle_left_sheet.png",
         "asset/santa_sheets/run_left_sheet.png"},
        {"asset/sprit/santa_sheets/idle_sheet.png",
         "asset/sprit/santa_sheets/run_sheet.png",
         "asset/sprit/santa_sheets/idle_left_sheet.png",
         "asset/sprit/santa_sheets/run_left_sheet.png"},
        {"asset/player/idle_sheet.png",
         "asset/player/run_sheet.png",
         "asset/player/idle_left_sheet.png",
         "asset/player/run_left_sheet.png"}};

    Animation *loadedIdle = NULL;
    Animation *loadedRun = NULL;

    for (int i = 0; i < 3 && (loadedIdle == NULL || loadedRun == NULL); i++)
    {
        if (loadedIdle == NULL)
        {
            loadedIdle = loadAnimation(renderer, paths[i][0], 6, 32, 32, 150);
            if (loadedIdle != NULL)
            {
                printf("Animation idle chargée avec succès: %s\n", paths[i][0]);
            }
        }

        if (loadedRun == NULL)
        {
            loadedRun = loadAnimation(renderer, paths[i][1], 6, 32, 32, 100);
            if (loadedRun != NULL)
            {
                printf("Animation run chargée avec succès: %s\n", paths[i][1]);
            }
        }
    }

    if (loadedIdle == NULL)
    {
        SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
        if (tempSurface != NULL)
        {
            SDL_FillRect(tempSurface, NULL, SDL_MapRGB(tempSurface->format, 255, 0, 0));

            loadedIdle = (Animation *)malloc(sizeof(Animation));
            if (loadedIdle != NULL)
            {
                loadedIdle->texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
                loadedIdle->frameCount = 1;
                loadedIdle->currentFrame = 0;
                loadedIdle->frameWidth = 32;
                loadedIdle->frameHeight = 32;
                loadedIdle->frameDuration = 150;
                loadedIdle->lastFrameTime = SDL_GetTicks();

                printf("Animation idle de secours créée\n");
            }
            SDL_FreeSurface(tempSurface);
        }
    }

    if (loadedRun == NULL && loadedIdle != NULL)
    {
        loadedRun = (Animation *)malloc(sizeof(Animation));
        if (loadedRun != NULL)
        {
            loadedRun->texture = loadedIdle->texture;
            loadedRun->frameCount = loadedIdle->frameCount;
            loadedRun->currentFrame = 0;
            loadedRun->frameWidth = loadedIdle->frameWidth;
            loadedRun->frameHeight = loadedIdle->frameHeight;
            loadedRun->frameDuration = 100;
            loadedRun->lastFrameTime = SDL_GetTicks();

            printf("Animation run de secours créée (basée sur idle)\n");
        }
    }

    if (loadedIdle == NULL || loadedRun == NULL)
    {
        printf("Erreur: Impossible de charger les animations de base\n");
        if (loadedIdle != NULL)
            freeAnimation(loadedIdle);
        if (loadedRun != NULL && loadedRun->texture != loadedIdle->texture)
            freeAnimation(loadedRun);
        free(animations);
        return NULL;
    }

    animations->idle = loadedIdle;
    animations->run = loadedRun;

    animations->idle_left = animations->idle;
    animations->run_left = animations->run;

    return animations;
}

void updateAnimation(Animation *animation)
{
    if (animation == NULL)
        return;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - animation->lastFrameTime > animation->frameDuration)
    {
        animation->currentFrame = (animation->currentFrame + 1) % animation->frameCount;
        animation->lastFrameTime = currentTime;
    }
}

void renderAnimation(SDL_Renderer *renderer, Animation *animation, int x, int y, int flipHorizontal)
{
    if (animation == NULL || renderer == NULL || animation->texture == NULL)
    {
        return;
    }

    SDL_Rect srcRect = {
        animation->currentFrame * animation->frameWidth,
        0,
        animation->frameWidth,
        animation->frameHeight};

    // Définir un facteur d'échelle pour chaque type d'animation
    float scaleFactor = 1.0f;

    // Si c'est une animation de pièce (coinAnimation ou starCoinAnimation), on la rend plus grande
    if (animation == coinAnimation || animation == starCoinAnimation)
    {
        scaleFactor = 1.5f; // Pièces 50% plus grandes
    }

    // Calculer la taille finale de l'animation
    int finalWidth = (int)(animation->frameWidth * scaleFactor);
    int finalHeight = (int)(animation->frameHeight * scaleFactor);

    SDL_Rect dstRect = {
        x - finalWidth / 2, // Centrer horizontalement
        y - finalHeight,    // Aligner en bas
        finalWidth,
        finalHeight};

    // Appliquer le flip horizontal si nécessaire
    SDL_RendererFlip flip = flipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    // Rendre l'animation
    SDL_RenderCopyEx(renderer, animation->texture, &srcRect, &dstRect, 0, NULL, flip);
}

void afficherPaysageSDL(Carte *carte, int positionJoueur, SDL_Renderer *renderer, int playerMoving)
{
    int largeurAffichage = 50;
    int demiLargeur = largeurAffichage / 2;

    int debutX = positionJoueur - demiLargeur;
    int finX = positionJoueur + demiLargeur;

    if (debutX < 0)
    {
        finX -= debutX;
        debutX = 0;
    }

    if (finX > carte->largeur)
    {
        debutX -= (finX - carte->largeur);
        finX = carte->largeur;
    }

    if (debutX < 0)
    {
        debutX = 0;
    }

    if (globalBackground == NULL)
    {
        const char *bgPath = "asset/sprit/Mondstadt Tileset Platform - Basic/windrise-background.png";
        globalBackground = loadBackgroundTexture(renderer, bgPath);

        if (globalBackground == NULL)
        {
            printf("Impossible de charger l'image PNG. Tentative avec un fichier BMP...\n");
            bgPath = "asset/sprit/Mondstadt Tileset Platform - Basic/windrise-background.png";
            globalBackground = loadBackgroundTexture(renderer, bgPath);
        }
    }

    if (gameTextures == NULL)
    {
        gameTextures = loadGameTextures(renderer);
        if (gameTextures == NULL)
        {
            printf("Impossible de charger les textures du jeu\n");
        }
    }

    if (playerAnimations == NULL)
    {
        playerAnimations = loadPlayerAnimations(renderer);
        if (playerAnimations == NULL)
        {
            printf("Impossible de charger les animations du joueur\n");
        }
    }

    // Charger les animations des pièces
    loadCoinAnimations(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (globalBackground != NULL && globalBackground->texture != NULL)
    {
        int backgroundOffset = (debutX * 3) % globalBackground->width;

        SDL_Rect srcRect = {backgroundOffset, 0, globalBackground->width - backgroundOffset, globalBackground->height};
        SDL_Rect dstRect = {0, 0, largeurAffichage * TILE_SIZE, 20 * TILE_SIZE};

        SDL_RenderCopy(renderer, globalBackground->texture, &srcRect, &dstRect);

        if (backgroundOffset > 0)
        {
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

            if (carte->carte[y][x] == 'M')
                continue;

            // On ne dessine plus directement les pièces ici (c et *)
            switch (carte->carte[y][x])
            {
            case 'w':
                if (gameTextures != NULL && gameTextures->brick != NULL)
                {
                    SDL_RenderCopy(renderer, gameTextures->brick, NULL, &tile);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
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
            case '!':
                SDL_SetRenderDrawColor(renderer, 25, 65, 199, 255);
                SDL_RenderFillRect(renderer, &tile);
                break;
            default:
                break;
            }
        }
    }

    // Dessiner les pièces animées
    animer_pieces(&tab_pieces);
    afficher_pieces(renderer, &tab_pieces, positionJoueur, debutX);

    for (int y = 0; y < carte->hauteur; y++)
    {
        for (int x = debutX; x < finX; x++)
        {
            if (x >= carte->largeur)
                continue;

            if (carte->carte[y][x] == 'M')
            {
                Animation *currentAnimation = NULL;

                if (playerAnimations != NULL)
                {

                    if (playerMoving && playerAnimations->run != NULL)
                    {
                        currentAnimation = playerAnimations->run;
                    }
                    else if (playerAnimations->idle != NULL)
                    {
                        currentAnimation = playerAnimations->idle;
                    }

                    if (currentAnimation != NULL)
                    {
                        updateAnimation(currentAnimation);

                        int screenX = (x - debutX) * TILE_SIZE + (TILE_SIZE / 2);
                        int screenY = y * TILE_SIZE + TILE_SIZE;

                        renderAnimation(renderer, currentAnimation, screenX, screenY, !playerAnimations->facingRight);
                    }
                }
                else
                {

                    SDL_Rect tile = {
                        (x - debutX) * TILE_SIZE,
                        y * TILE_SIZE,
                        TILE_SIZE,
                        TILE_SIZE};
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
            }
        }
    }
}
void freePlayerAnimations(PlayerAnimations *animations)
{
    if (animations != NULL)
    {
        // Free the idle animation
        if (animations->idle != NULL)
        {
            // Free the texture only if it's not shared
            if (animations->idle->texture != NULL &&
                (animations->idle_left == NULL || animations->idle->texture != animations->idle_left->texture) &&
                (animations->run == NULL || animations->idle->texture != animations->run->texture) &&
                (animations->run_left == NULL || animations->idle->texture != animations->run_left->texture))
            {
                SDL_DestroyTexture(animations->idle->texture);
            }
            free(animations->idle);
        }

        // Free the idle_left animation
        if (animations->idle_left != NULL && animations->idle_left != animations->idle)
        {
            // Only free texture if it's not shared with another animation
            if (animations->idle_left->texture != NULL &&
                (animations->run == NULL || animations->idle_left->texture != animations->run->texture) &&
                (animations->run_left == NULL || animations->idle_left->texture != animations->run_left->texture))
            {
                SDL_DestroyTexture(animations->idle_left->texture);
            }
            free(animations->idle_left);
        }

        // Free the run animation
        if (animations->run != NULL && animations->run != animations->idle && animations->run != animations->idle_left)
        {
            // Only free texture if it's not shared with run_left
            if (animations->run->texture != NULL &&
                (animations->run_left == NULL || animations->run->texture != animations->run_left->texture))
            {
                SDL_DestroyTexture(animations->run->texture);
            }
            free(animations->run);
        }

        // Free the run_left animation
        if (animations->run_left != NULL &&
            animations->run_left != animations->idle &&
            animations->run_left != animations->idle_left &&
            animations->run_left != animations->run)
        {
            if (animations->run_left->texture != NULL)
            {
                SDL_DestroyTexture(animations->run_left->texture);
            }
            free(animations->run_left);
        }

        // Finally, free the animations struct
        free(animations);
    }
}
// Modification de nettoyerSDL dans affichage.c
void nettoyerSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    liberer_pieces(&tab_pieces);

    if (playerAnimations != NULL)
    {
        freePlayerAnimations(playerAnimations);
        playerAnimations = NULL;
    }

    if (gameTextures != NULL)
    {
        freeGameTextures(gameTextures);
        gameTextures = NULL;
    }

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
void freeAnimation(Animation *animation)
{
    if (animation != NULL)
    {
        if (animation->texture != NULL)
        {
            SDL_DestroyTexture(animation->texture);
        }
        free(animation);
    }
}