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
        
        const char* bgPath = "asset/sprit/Mondstadt Tileset Platform - Basic/windrise-background.png";
        globalBackground = loadBackgroundTexture(renderer, bgPath);
        
        
        if (globalBackground == NULL) {
            printf("Impossible de charger l'image PNG. Tentative avec un fichier BMP...\n");
            bgPath = "asset/sprit/Mondstadt Tileset Platform - Basic/windrise-background.png";
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