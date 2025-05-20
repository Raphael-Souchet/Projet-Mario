#include "projet.h"

GameTextures *loadGameTextures(SDL_Renderer *renderer)
{
    GameTextures *textures = (GameTextures *)malloc(sizeof(GameTextures));
    if (textures == NULL)
    {
        return NULL;
    }

    textures->brick = NULL;
    textures->terre = NULL;  
    textures->tuyau = NULL;
    textures->plante = NULL;  
    textures->herbe3 = NULL;  
    textures->terre3 = NULL;
    textures->herbe4 = NULL;  
    textures->terre4 = NULL;
    textures->herbe5 = NULL;  
    textures->terre5 = NULL;

    SDL_Surface *surface_brick = IMG_Load("asset/sprit/Tiles/herbe.png");
    
    if (surface_brick != NULL)
    {
        textures->brick = SDL_CreateTextureFromSurface(renderer, surface_brick);
        SDL_FreeSurface(surface_brick);
    }

    
    SDL_Surface *surface_terre = IMG_Load("asset/sprit/Tiles/terre.png");
    
    if (surface_terre != NULL)
    {
        textures->terre = SDL_CreateTextureFromSurface(renderer, surface_terre);
        SDL_FreeSurface(surface_terre);
    }

    SDL_Surface *surface_tuyau = IMG_Load("asset/Tiles/tuyau.png");
    if (surface_tuyau != NULL)
    {
        textures->tuyau = SDL_CreateTextureFromSurface(renderer, surface_tuyau);
        SDL_FreeSurface(surface_tuyau);
    }

    SDL_Surface *surface_plante = IMG_Load("asset/Tiles/plante.png");
    
    if (surface_plante != NULL)
    {
        textures->plante = SDL_CreateTextureFromSurface(renderer, surface_plante);
        SDL_FreeSurface(surface_plante);
    }

    SDL_Surface *surface_nuage = IMG_Load("asset/Tiles/nuage.png");
    
    if (surface_nuage != NULL)
    {
        textures->nuage = SDL_CreateTextureFromSurface(renderer, surface_nuage);
        SDL_FreeSurface(surface_nuage);
    }

    SDL_Surface *surface_herbe5 = IMG_Load("asset/Tiles/herbe5.png");
    
    if (surface_herbe5 != NULL)
    {
        textures->herbe5 = SDL_CreateTextureFromSurface(renderer, surface_herbe5);
        SDL_FreeSurface(surface_herbe5);
    }

    SDL_Surface *surface_terre5 = IMG_Load("asset/Tiles/terre5.png");
    
    if (surface_terre5 != NULL)
    {
        textures->terre5 = SDL_CreateTextureFromSurface(renderer, surface_terre5);
        SDL_FreeSurface(surface_terre5);
    }


    SDL_Surface *surface_herbe4 = IMG_Load("asset/Tiles/herbe4.png");
    if (surface_herbe4 != NULL)
    {
        textures->herbe4 = SDL_CreateTextureFromSurface(renderer, surface_herbe4);
        SDL_FreeSurface(surface_herbe4);
    }

    SDL_Surface *surface_terre4 = IMG_Load("asset/Tiles/terre4.png");
    
    if (surface_terre4 != NULL)
    {
        textures->terre4 = SDL_CreateTextureFromSurface(renderer, surface_terre4);
        SDL_FreeSurface(surface_terre4);
    }


    
    SDL_Surface *surface_herbe3 = IMG_Load("asset/Tiles/herbe3.png");
    
    if (surface_herbe3 != NULL)
    {
        textures->herbe3 = SDL_CreateTextureFromSurface(renderer, surface_herbe3);
        SDL_FreeSurface(surface_herbe3);
    }

    SDL_Surface *surface_terre3 = IMG_Load("asset/Tiles/terre3.png");
    
    if (surface_terre3 != NULL)
    {
        textures->terre3 = SDL_CreateTextureFromSurface(renderer, surface_terre3);
        SDL_FreeSurface(surface_terre3);
    }

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
        if (textures->terre != NULL)
        {
            SDL_DestroyTexture(textures->terre);
        }
        free(textures);
    }
}
