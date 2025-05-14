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

// Nouvelle fonction pour charger les textures du jeu
GameTextures* loadGameTextures(SDL_Renderer *renderer)
{
    GameTextures* textures = (GameTextures*)malloc(sizeof(GameTextures));
    if (textures == NULL) {
        printf("Erreur: Impossible d'allouer la mémoire pour les textures du jeu\n");
        return NULL;
    }
    
    // Initialisation par défaut
    textures->brick = NULL;
    
    // Chargement de la texture pour les briques
    SDL_Surface* surface = IMG_Load("asset/sprit/tiles/tile_0004.png");
    if (surface == NULL) {
        printf("Erreur: Impossible de charger l'image tile_0004.png: %s\n", IMG_GetError());
        // Tentative avec un chemin alternatif
        surface = IMG_Load("asset/tiles/tile_0004.png");
        if (surface == NULL) {
            printf("Erreur: Impossible de charger l'image alternative: %s\n", IMG_GetError());
            free(textures);
            return NULL;
        }
    }
    
    textures->brick = SDL_CreateTextureFromSurface(renderer, surface);
    if (textures->brick == NULL) {
        printf("Erreur: Impossible de créer la texture de brique: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(textures);
        return NULL;
    }
    
    SDL_FreeSurface(surface);
    
    return textures;
}

// Fonction pour libérer les textures du jeu
void freeGameTextures(GameTextures* textures)
{
    if (textures != NULL) {
        if (textures->brick != NULL) {
            SDL_DestroyTexture(textures->brick);
        }
        // Libérer d'autres textures si ajoutées
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
Animation* loadAnimation(SDL_Renderer* renderer, const char* path, int frameCount, int frameWidth, int frameHeight, Uint32 frameDuration) {
    Animation* animation = (Animation*)malloc(sizeof(Animation));
    if (animation == NULL) {
        printf("Erreur: Impossible d'allouer la mémoire pour l'animation\n");
        return NULL;
    }
    
    // Initialisation par défaut
    animation->texture = NULL;
    animation->frameCount = frameCount;
    animation->currentFrame = 0;
    animation->frameWidth = frameWidth;
    animation->frameHeight = frameHeight;
    animation->frameDuration = frameDuration;
    animation->lastFrameTime = SDL_GetTicks();
    
    // Charger la texture
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL) {
        printf("Erreur: Impossible de charger l'image %s: %s\n", path, IMG_GetError());
        free(animation);
        return NULL;
    }
    
    animation->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (animation->texture == NULL) {
        printf("Erreur: Impossible de créer la texture d'animation: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(animation);
        return NULL;
    }
    
    SDL_FreeSurface(surface);
    
    return animation;
}
PlayerAnimations* loadPlayerAnimations(SDL_Renderer* renderer) {
    // Allocation de la mémoire pour la structure d'animations
    PlayerAnimations* animations = (PlayerAnimations*)malloc(sizeof(PlayerAnimations));
    if (animations == NULL) {
        printf("Erreur: Impossible d'allouer la mémoire pour les animations du joueur\n");
        return NULL;
    }
    
    // Initialisation des valeurs par défaut
    animations->idle = NULL;
    animations->run = NULL;
    animations->facingRight = 1; // 1 = face à droite (par défaut), 0 = face à gauche
    
    // Chargement de l'animation d'attente (idle)
    animations->idle = loadAnimation(renderer, "asset/santa_sheets/idle_sheet.png", 6, 32, 32, 150);
    if (animations->idle == NULL) {
        printf("Erreur: Impossible de charger l'animation idle\n");
        free(animations);
        return NULL;
    }
    
    // Chargement de l'animation de course (run)
    animations->run = loadAnimation(renderer, "asset/santa_sheets/run_sheet.png", 6, 32, 32, 100);
    if (animations->run == NULL) {
        printf("Erreur: Impossible de charger l'animation de course\n");
        freeAnimation(animations->idle);
        free(animations);
        return NULL;
    }
    
    
    return animations;
}

void updateAnimation(Animation* animation) {
    if (animation == NULL) return;
    
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - animation->lastFrameTime > animation->frameDuration) {
        animation->currentFrame = (animation->currentFrame + 1) % animation->frameCount;
        animation->lastFrameTime = currentTime;
    }
}

void renderAnimation(SDL_Renderer* renderer, Animation* animation, int x, int y, int flipHorizontal) {
    if (animation == NULL || renderer == NULL) return;
    
    // Rectangle source dans la texture (frame courante)
    SDL_Rect srcRect = {
        animation->currentFrame * animation->frameWidth,
        0,
        animation->frameWidth,
        animation->frameHeight
    };
    
    // Rectangle destination sur l'écran (position centrée horizontalement)
    SDL_Rect dstRect = {
        x - animation->frameWidth / 2,  // Centrer horizontalement
        y - animation->frameHeight,     // Aligner le bas de l'animation avec la position y
        animation->frameWidth,
        animation->frameHeight
    };
    
    // Appliquer le flip horizontal si nécessaire
    SDL_RendererFlip flip = flipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    
    // Vérifier que le renderer et la texture sont valides avant de rendre
    if (renderer != NULL && animation->texture != NULL) {
        SDL_RenderCopyEx(renderer, animation->texture, &srcRect, &dstRect, 0, NULL, flip);
    }
}

void freePlayerAnimations(PlayerAnimations* animations) {
    if (animations != NULL) {
        if (animations->idle != NULL) {
            freeAnimation(animations->idle);
        }
        if (animations->run != NULL) {
            freeAnimation(animations->run);
        }
        free(animations);
    }
}


// Modification de la fonction afficherPaysageSDL pour utiliser l'animation
void afficherPaysageSDL(Carte *carte, int positionJoueur, SDL_Renderer *renderer, int playerMoving) {
    int largeurAffichage = 50;
    int demiLargeur = largeurAffichage / 2;
    
    // Calcul des limites d'affichage
    int debutX = positionJoueur - demiLargeur;
    int finX = positionJoueur + demiLargeur;
    
    // Ajustements des limites si nécessaire
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
    
    // Chargement du fond d'écran si nécessaire
    if (globalBackground == NULL) {
        const char* bgPath = "asset/sprit/Mondstadt Tileset Platform - Basic/windrise-background.png";
        globalBackground = loadBackgroundTexture(renderer, bgPath);
        
        if (globalBackground == NULL) {
            printf("Impossible de charger l'image PNG. Tentative avec un fichier BMP...\n");
            bgPath = "asset/sprit/Mondstadt Tileset Platform - Basic/windrise-background.png";
            globalBackground = loadBackgroundTexture(renderer, bgPath);
        }
    }
    
    // Chargement des textures du jeu si nécessaire
    if (gameTextures == NULL) {
        gameTextures = loadGameTextures(renderer);
        if (gameTextures == NULL) {
            printf("Impossible de charger les textures du jeu\n");
        }
    }
    
    // Chargement des animations du joueur si nécessaire
    if (playerAnimations == NULL) {
        playerAnimations = loadPlayerAnimations(renderer);
        if (playerAnimations == NULL) {
            printf("Impossible de charger les animations du joueur\n");
        }
    }

    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Afficher le fond d'écran
    if (globalBackground != NULL && globalBackground->texture != NULL) {
        int backgroundOffset = (debutX * 3) % globalBackground->width;  
        
        SDL_Rect srcRect = {backgroundOffset, 0, globalBackground->width - backgroundOffset, globalBackground->height};
        SDL_Rect dstRect = {0, 0, largeurAffichage * TILE_SIZE, 20 * TILE_SIZE};
        
        SDL_RenderCopy(renderer, globalBackground->texture, &srcRect, &dstRect);
        
        // Si on a besoin de boucler l'arrière-plan
        if (backgroundOffset > 0) {
            srcRect.x = 0;
            srcRect.w = backgroundOffset;
            dstRect.x = (globalBackground->width - backgroundOffset) * (largeurAffichage * TILE_SIZE) / globalBackground->width;
            dstRect.w = largeurAffichage * TILE_SIZE - dstRect.x;
            SDL_RenderCopy(renderer, globalBackground->texture, &srcRect, &dstRect);
        }
    }

    // Afficher les éléments de la carte
    for (int y = 0; y < carte->hauteur; y++) {
        for (int x = debutX; x < finX; x++) {
            if (x >= carte->largeur)
                continue;

            SDL_Rect tile = {
                (x - debutX) * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE};

            // Ne pas dessiner le joueur ici car nous utiliserons l'animation
            if (carte->carte[y][x] == 'M')
                continue;

            switch (carte->carte[y][x]) {
            case 'w':
                // Utiliser la texture de brique pour les 'w' au lieu d'un rectangle coloré
                if (gameTextures != NULL && gameTextures->brick != NULL) {
                    SDL_RenderCopy(renderer, gameTextures->brick, NULL, &tile);
                } else {
                    // Fallback si la texture n'est pas disponible
                    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
                break;
            case 'c':
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
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
                // Espace vide ou autre caractère non défini
                break;
            }
        }
    }
    
    // Rechercher et afficher le joueur avec l'animation appropriée
    for (int y = 0; y < carte->hauteur; y++) {
        for (int x = debutX; x < finX; x++) {
            if (x >= carte->largeur)
                continue;
                
            if (carte->carte[y][x] == 'M') {
                // Choisir l'animation appropriée selon l'état du personnage
                Animation* currentAnimation = NULL;
                
                if (playerAnimations != NULL) {
                    if (playerMoving && playerAnimations->run != NULL) {
                        currentAnimation = playerAnimations->run;
                    } else if (playerAnimations->idle != NULL) {
                        currentAnimation = playerAnimations->idle;
                    }
                    
                    if (currentAnimation != NULL) {
                        updateAnimation(currentAnimation);
                        
                        // Rendre l'animation du joueur à sa position dans la carte
                        int screenX = (x - debutX) * TILE_SIZE + (TILE_SIZE / 2);
                        int screenY = y * TILE_SIZE + TILE_SIZE;
                        
                        // Utiliser le flip horizontal selon la direction du personnage
                        // Si facingRight = 0 (regardant à gauche), appliquer le flip horizontal
                        // Si facingRight = 1 (regardant à droite), ne pas appliquer de flip
                        renderAnimation(renderer, currentAnimation, screenX, screenY, !playerAnimations->facingRight);
                    }
                } else {
                    // Fallback si l'animation n'est pas disponible
                    SDL_Rect tile = {
                        (x - debutX) * TILE_SIZE,
                        y * TILE_SIZE,
                        TILE_SIZE,
                        TILE_SIZE
                    };
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
            }
        }
    }
}


// Modifier la fonction nettoyerSDL pour libérer les ressources d'animation
void nettoyerSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    // Libérer les animations du joueur
    if (playerAnimations != NULL)
    {
        freePlayerAnimations(playerAnimations);
        playerAnimations = NULL;
    }
    
    // Libérer les textures du jeu
    if (gameTextures != NULL)
    {
        freeGameTextures(gameTextures);
        gameTextures = NULL;
    }
    
    // Libérer la texture de fond
    if (globalBackground != NULL)
    {
        freeBackgroundTexture(globalBackground);
        globalBackground = NULL;  
    }
    
    // Libérer le rendu et la fenêtre
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
void freeAnimation(Animation* animation) {
    if (animation != NULL) {
        if (animation->texture != NULL) {
            SDL_DestroyTexture(animation->texture);
        }
        free(animation);
    }
}
