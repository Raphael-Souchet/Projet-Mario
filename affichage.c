#include "projet.h"
Animation* flagAnimation = NULL;

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
    textures->terre = NULL;  // Initialisation de terre à NULL

    // Chargement de la texture herbe
    SDL_Surface *surface = IMG_Load("asset/sprit/tiles/herbe.png");
    if (surface == NULL)
    {
        printf("Erreur: Impossible de charger l'image herbe.png: %s\n", IMG_GetError());
        surface = IMG_Load("asset/tiles/herbe.png");
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
    
    SDL_FreeSurface(surface);  // Libération de la surface herbe
    
    // Chargement de la texture terre (correction)
    SDL_Surface *surface_terre = IMG_Load("asset/sprit/tiles/terre.png");
    if (surface_terre == NULL)
    {
        printf("Erreur: Impossible de charger l'image terre.png: %s\n", IMG_GetError());
        surface_terre = IMG_Load("asset/tiles/terre.png");
        if (surface_terre == NULL)
        {
            printf("Erreur: Impossible de charger l'image terre alternative: %s\n", IMG_GetError());
            // On continue car la texture herbe est déjà chargée
        }
    }
    
    if (surface_terre != NULL)
    {
        textures->terre = SDL_CreateTextureFromSurface(renderer, surface_terre);
        if (textures->terre == NULL)
        {
            printf("Erreur: Impossible de créer la texture de terre: %s\n", SDL_GetError());
            // On continue car la texture herbe est déjà chargée
        }
        SDL_FreeSurface(surface_terre);
    }

    return textures;
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
    if (coinAnimation == NULL)
    {
        const char *coinPaths[] = {
            "asset/brackeys_platformer_assets/sprites/coin.png",
            "asset/sprites/coin.png",
            "asset/sprit/sprites/coin.png",
            "asset/coin.png"};

        for (int i = 0; i < 4 && coinAnimation == NULL; i++)
        {
            coinAnimation = loadAnimation(renderer, coinPaths[i], 6, 16, 16, 100);
            if (coinAnimation != NULL)
            {
                printf("Animation des pièces chargée avec succès: %s\n", coinPaths[i]);
            }
        }

        if (coinAnimation == NULL)
        {
            printf("Échec du chargement de l'animation des pièces. Utilisation de la représentation par défaut.\n");
        }
    }

    if (starCoinAnimation == NULL)
    {
        const char *starCoinPaths[] = {
            "asset/brackeys_platformer_assets/sprites/starcoin.png",
            "asset/sprites/starcoin.png",
            "asset/sprit/sprites/starcoin.png",
            "asset/starcoin.png",
            "asset/starcoin.png"};

        for (int i = 0; i < 5 && starCoinAnimation == NULL; i++)
        {
            starCoinAnimation = loadAnimation(renderer, starCoinPaths[i], 6, 16, 16, 100);
            if (starCoinAnimation != NULL)
            {
                printf("Animation des pièces étoiles chargée avec succès: %s\n", starCoinPaths[i]);
            }
        }

        if (starCoinAnimation == NULL)
        {
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
void loadFlagAnimation(SDL_Renderer *renderer)
{
    if (flagAnimation == NULL)
    {
        const char *flagPaths[] = {
            "C:/Users/duber/cours L1/programation/mario_vscode/Projet-Mario/asset/Rocky Roads/Objects/flag.png",
            "asset/Rocky Roads/Objects/flag.png",
            "asset/sprit/objects/flag.png",
            "asset/flag.png"
        };

        for (int i = 0; i < 4 && flagAnimation == NULL; i++)
        {
            flagAnimation = loadAnimation(renderer, flagPaths[i], 4, 32, 32, 200);
            if (flagAnimation != NULL)
            {
                printf("Animation du drapeau chargée avec succès: %s\n", flagPaths[i]);
                break;
            }
        }

        if (flagAnimation == NULL)
        {
            printf("Échec du chargement de l'animation du drapeau. Utilisation de la représentation par défaut.\n");
            // Création d'une texture de secours si nécessaire
            SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
            if (tempSurface != NULL)
            {
                SDL_FillRect(tempSurface, NULL, SDL_MapRGB(tempSurface->format, 0, 100, 255));
                
                flagAnimation = (Animation *)malloc(sizeof(Animation));
                if (flagAnimation != NULL)
                {
                    flagAnimation->texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
                    flagAnimation->frameCount = 1;
                    flagAnimation->currentFrame = 0;
                    flagAnimation->frameWidth = 32;
                    flagAnimation->frameHeight = 32;
                    flagAnimation->frameDuration = 200;
                    flagAnimation->lastFrameTime = SDL_GetTicks();
                    
                    printf("Animation de drapeau de secours créée\n");
                }
                SDL_FreeSurface(tempSurface);
            }
        }
    }
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

    float scaleFactor = 1.0f;

    if (animation == coinAnimation || animation == starCoinAnimation)
    {
        scaleFactor = 1.5f; 
    }

    int finalWidth = (int)(animation->frameWidth * scaleFactor);
    int finalHeight = (int)(animation->frameHeight * scaleFactor);

    SDL_Rect dstRect = {
        x - finalWidth / 2, 
        y - finalHeight,  
        finalWidth,
        finalHeight};

    SDL_RendererFlip flip = flipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    SDL_RenderCopyEx(renderer, animation->texture, &srcRect, &dstRect, 0, NULL, flip);
}

TTF_Font* scoreFont = NULL;
SDL_Color scoreColor = {255, 255, 255, 255};

TTF_Font* initFont() {
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de TTF: %s\n", TTF_GetError());
        return NULL;
    }
    
    const char* fontPaths[] = {
        "asset/font/PixelifySans-VariableFont_wght.ttf"
    };
    
    TTF_Font* font = NULL;
    for (int i = 0; i < 3 && font == NULL; i++) {
        font = TTF_OpenFont(fontPaths[i], 32); 
        if (font != NULL) {
            printf("Police chargée avec succès: %s\n", fontPaths[i]);
            break;
        }
    }
    
    if (font == NULL) {
        printf("Impossible de charger la police: %s\n", TTF_GetError());
    }
    
    return font;
}

void afficherScore(SDL_Renderer* renderer, int score) {
    if (scoreFont == NULL) {
        return;
    }
    
    char scoreText[50];
    sprintf(scoreText, "Score: %d", score);
    
    SDL_Color scoreColor = {255, 255, 0, 255}; 
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(scoreFont, scoreText, scoreColor);
    if (textSurface == NULL) {
        printf("Impossible de créer la surface de texte: %s\n", TTF_GetError());
        return;
    }
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        printf("Impossible de créer la texture de texte: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }
    
    SDL_Rect destRect = {40, 30, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);
    
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void afficherPaysageSDL(Carte *carte, int positionJoueur, SDL_Renderer *renderer, int playerMoving, int score)
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

    loadCoinAnimations(renderer);
    loadFlagAnimation(renderer);  // Chargement de l'animation du drapeau

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (globalBackground != NULL && globalBackground->texture != NULL)
    {
        int backgroundOffset = (debutX * 3) % globalBackground->width;

        SDL_Rect srcRect = {backgroundOffset, 0, globalBackground->width - backgroundOffset, globalBackground->height};
        SDL_Rect dstRect = {0, 0, largeurAffichage * TILE_SIZE, 24 * TILE_SIZE};

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
                // Au lieu de dessiner un rectangle, on va animer le drapeau
                if (flagAnimation != NULL)
                {
                    updateAnimation(flagAnimation);
                    int screenX = (x - debutX) * TILE_SIZE + (TILE_SIZE / 2);
                    int screenY = y * TILE_SIZE + TILE_SIZE;
                    renderAnimation(renderer, flagAnimation, screenX, screenY, 0);
                }
                else
                {
                    // Fallback si l'animation ne peut pas être chargée
                    SDL_SetRenderDrawColor(renderer, 25, 65, 199, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
                break;
            case 'd':
                if (gameTextures != NULL && gameTextures->terre != NULL)
                {
                    SDL_RenderCopy(renderer, gameTextures->terre, NULL, &tile);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
                break;
            default:
                break;
            }
        }
    }

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
    
    afficherScore(renderer, score);
}

void freePlayerAnimations(PlayerAnimations *animations)
{
    if (animations != NULL)
    {
        if (animations->idle != NULL)
        {
            if (animations->idle->texture != NULL &&
                (animations->idle_left == NULL || animations->idle->texture != animations->idle_left->texture) &&
                (animations->run == NULL || animations->idle->texture != animations->run->texture) &&
                (animations->run_left == NULL || animations->idle->texture != animations->run_left->texture))
            {
                SDL_DestroyTexture(animations->idle->texture);
            }
            free(animations->idle);
        }

        if (animations->idle_left != NULL && animations->idle_left != animations->idle)
        {
            if (animations->idle_left->texture != NULL &&
                (animations->run == NULL || animations->idle_left->texture != animations->run->texture) &&
                (animations->run_left == NULL || animations->idle_left->texture != animations->run_left->texture))
            {
                SDL_DestroyTexture(animations->idle_left->texture);
            }
            free(animations->idle_left);
        }

        if (animations->run != NULL && animations->run != animations->idle && animations->run != animations->idle_left)
        {
            if (animations->run->texture != NULL &&
                (animations->run_left == NULL || animations->run->texture != animations->run_left->texture))
            {
                SDL_DestroyTexture(animations->run->texture);
            }
            free(animations->run);
        }

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

        free(animations);
    }
}
void freeGameTextures(GameTextures *textures)
{
    if (textures != NULL)
    {
        if (textures->brick != NULL)
        {
            SDL_DestroyTexture(textures->brick);
        }
        if (textures->terre != NULL)
        {
            SDL_DestroyTexture(textures->terre);
        }
        free(textures);
    }
}
void freeFlagAnimation()
{
    if (flagAnimation != NULL)
    {
        freeAnimation(flagAnimation);
        flagAnimation = NULL;
    }
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
void nettoyerSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    liberer_pieces(&tab_pieces);

    if (scoreFont != NULL) {
        TTF_CloseFont(scoreFont);
        scoreFont = NULL;
        TTF_Quit();
    }

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
    
    // Libération de l'animation du drapeau
    if (flagAnimation != NULL)
    {
        freeAnimation(flagAnimation);
        flagAnimation = NULL;
    }
    
    // Libération des animations de pièces
    if (coinAnimation != NULL)
    {
        freeAnimation(coinAnimation);
        coinAnimation = NULL;
    }
    
    if (starCoinAnimation != NULL)
    {
        freeAnimation(starCoinAnimation);
        starCoinAnimation = NULL;
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
