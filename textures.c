#include "projet.h"

GameTextures *loadGameTextures(SDL_Renderer *renderer)
{
    GameTextures *textures = (GameTextures *)malloc(sizeof(GameTextures));
    if (textures == NULL)
    {
        printf("Erreur: Impossible d'allouer la mémoire pour les textures du jeu\n");
        return NULL;
    }

    textures->brick = NULL;
    textures->terre = NULL;  
    textures->tuyau = NULL;
    textures->plante = NULL;  

    SDL_Surface *surface = IMG_Load("asset/sprit/Tiles/herbe.png");
    if (surface == NULL)
    {
        printf("Erreur: Impossible de charger l'image herbe.png: %s\n", IMG_GetError());
        surface = IMG_Load("asset/Tiles/herbe.png");
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
    
    SDL_Surface *surface_terre = IMG_Load("asset/sprit/Tiles/terre.png");
    if (surface_terre == NULL)
    {
        printf("Erreur: Impossible de charger l'image terre.png: %s\n", IMG_GetError());
        surface_terre = IMG_Load("asset/Tiles/terre.png");
        if (surface_terre == NULL)
        {
            printf("Erreur: Impossible de charger l'image terre alternative: %s\n", IMG_GetError());
        }
    }
    
    if (surface_terre != NULL)
    {
        textures->terre = SDL_CreateTextureFromSurface(renderer, surface_terre);
        if (textures->terre == NULL)
        {
            printf("Erreur: Impossible de créer la texture de terre: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(surface_terre);
    }

    SDL_Surface *surface_tuyau = IMG_Load("asset/Tiles/tuyau.png");
    if (surface_tuyau == NULL)
    {
        printf("Erreur: Impossible de charger l'image tuyau.png: %s\n", IMG_GetError());
        surface_terre = IMG_Load("asset/Tiles/tuyau.png");
        if (surface_tuyau == NULL)
        {
            printf("Erreur: Impossible de charger l'image tuyau alternative: %s\n", IMG_GetError());
        }
    }
    
    if (surface_tuyau != NULL)
    {
        textures->tuyau = SDL_CreateTextureFromSurface(renderer, surface_tuyau);
        if (textures->tuyau == NULL)
        {
            printf("Erreur: Impossible de créer la texture de tuyau: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(surface_tuyau);
    }

    SDL_Surface *surface_plante = IMG_Load("asset/Tiles/plante.png");
    if (surface_plante == NULL)
    {
        printf("Erreur: Impossible de charger l'image plante.png: %s\n", IMG_GetError());
        surface_plante = IMG_Load("asset/Tiles/plante.png");
        if (surface_plante == NULL)
        {
            printf("Erreur: Impossible de charger l'image plante alternative: %s\n", IMG_GetError());
        }
    }
    
    if (surface_plante != NULL)
    {
        textures->plante = SDL_CreateTextureFromSurface(renderer, surface_plante);
        if (textures->plante == NULL)
        {
            printf("Erreur: Impossible de créer la texture de plante: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(surface_plante);
    }

    SDL_Surface *surface_nuage = IMG_Load("asset/Tiles/nuage.png");
    if (surface_nuage == NULL)
    {
        printf("Erreur: Impossible de charger l'image nuage.png: %s\n", IMG_GetError());
        surface_nuage = IMG_Load("asset/Tiles/nuage.png");
        if (surface_nuage == NULL)
        {
            printf("Erreur: Impossible de charger l'image nuage alternative: %s\n", IMG_GetError());
        }
    }
    
    if (surface_nuage != NULL)
    {
        textures->nuage = SDL_CreateTextureFromSurface(renderer, surface_nuage);
        if (textures->nuage == NULL)
        {
            printf("Erreur: Impossible de créer la texture du nuage: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(surface_nuage);
    }

    SDL_Surface *surface_herbe5 = IMG_Load("asset/Tiles/herbe5.png");
    if (surface_herbe5 == NULL)
    {
        printf("Erreur: Impossible de charger l'image nuage.png: %s\n", IMG_GetError());
        surface_herbe5 = IMG_Load("asset/Tiles/herbe5.png");
        if (surface_herbe5 == NULL)
        {
            printf("Erreur: Impossible de charger l'image nuage alternative: %s\n", IMG_GetError());
        }
    }
    
    if (surface_herbe5 != NULL)
    {
        textures->herbe5 = SDL_CreateTextureFromSurface(renderer, surface_herbe5);
        if (textures->herbe5 == NULL)
        {
            printf("Erreur: Impossible de créer la texture du nuage: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(surface_herbe5);
    }

    SDL_Surface *surface_terre5 = IMG_Load("asset/Tiles/terre5.png");
    if (surface_terre5 == NULL)
    {
        printf("Erreur: Impossible de charger l'image nuage.png: %s\n", IMG_GetError());
        surface_terre5 = IMG_Load("asset/Tiles/terre5.png");
        if (surface_terre5 == NULL)
        {
            printf("Erreur: Impossible de charger l'image nuage alternative: %s\n", IMG_GetError());
        }
    }
    
    if (surface_terre5 != NULL)
    {
        textures->terre5 = SDL_CreateTextureFromSurface(renderer, surface_terre5);
        if (textures->terre5 == NULL)
        {
            printf("Erreur: Impossible de créer la texture du nuage: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(surface_terre5);
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
