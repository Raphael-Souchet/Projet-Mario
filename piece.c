#include "projet.h"

Tab_piece tab_pieces = {NULL, 0, 0};
Animation* coinAnimation = NULL;
Animation* starCoinAnimation = NULL;

void initialiser_pieces(Carte* carte, Tab_piece* tab_piece) {
    int nb_pieces = 0;
    for (int y = 0; y < carte->hauteur; y++) {
        for (int x = 0; x < carte->largeur; x++) {
            if (carte->carte[y][x] == 'c' || carte->carte[y][x] == '*') {
                nb_pieces++;
            }
        }
    }
    
    tab_piece->capacity = nb_pieces + 10; 
    tab_piece->pieces = (Piece*)malloc(sizeof(Piece) * tab_piece->capacity);
    tab_piece->count = 0;
    
    if (tab_piece->pieces == NULL) {
        printf("Erreur: impossible d'allouer la mémoire pour les pièces\n");
        return;
    }
    
    for (int y = 0; y < carte->hauteur; y++) {
        for (int x = 0; x < carte->largeur; x++) {
            if (carte->carte[y][x] == 'c' || carte->carte[y][x] == '*') {
                tab_piece->pieces[tab_piece->count].positionX = x;
                tab_piece->pieces[tab_piece->count].positionY = y;
                tab_piece->pieces[tab_piece->count].type = (carte->carte[y][x] == '*') ? 1 : 0;
                tab_piece->pieces[tab_piece->count].actif = 1;
                tab_piece->count++;
            }
        }
    }
}

void animer_pieces(Tab_piece* tab_piece) {
    if (coinAnimation != NULL) {
        updateAnimation(coinAnimation);
    }
    
    if (starCoinAnimation != NULL) {
        updateAnimation(starCoinAnimation);
    }
}

void afficher_pieces(SDL_Renderer* renderer, Tab_piece* tab_piece, int positionJoueur, int debutX) {
    int largeurAffichage = 50;
    int finX = debutX + largeurAffichage;

    for (int i = 0; i < tab_piece->count; i++) {
        if (!tab_piece->pieces[i].actif) continue;
        
        int pieceX = tab_piece->pieces[i].positionX;
        int pieceY = tab_piece->pieces[i].positionY;
        
        if (pieceX >= debutX && pieceX < finX) {
            Animation* currentAnimation = (tab_piece->pieces[i].type == 1) ? starCoinAnimation : coinAnimation;
            
            if (currentAnimation != NULL) {
                int screenX = (pieceX - debutX) * TILE_SIZE + (TILE_SIZE / 2);
                int screenY = pieceY * TILE_SIZE + (TILE_SIZE / 2);
                
                renderAnimation(renderer, currentAnimation, screenX, screenY, 0);
            } else {
                SDL_Rect tile = {
                    (pieceX - debutX) * TILE_SIZE,
                    pieceY * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };
                
                if (tab_piece->pieces[i].type == 1) {
                    SDL_SetRenderDrawColor(renderer, 241, 255, 65, 255);
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);   
                }
                
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }
}

void liberer_pieces(Tab_piece* tab_piece) {
    if (tab_piece->pieces != NULL) {
        free(tab_piece->pieces);
        tab_piece->pieces = NULL;
    }
    tab_piece->count = 0;
    tab_piece->capacity = 0;
    
    if (coinAnimation != NULL) {
        freeAnimation(coinAnimation);
        coinAnimation = NULL;
    }
    
    if (starCoinAnimation != NULL) {
        freeAnimation(starCoinAnimation);
        starCoinAnimation = NULL;
    }
}

void check_collect_piece(Carte* carte, Tab_piece* tab_piece, Personnage* perso) {
    for (int i = 0; i < tab_piece->count; i++) {
        if (tab_piece->pieces[i].actif && 
            tab_piece->pieces[i].positionX == perso->positionX && 
            tab_piece->pieces[i].positionY == perso->positionY) {
            
            tab_piece->pieces[i].actif = 0;
            
            if (tab_piece->pieces[i].type == 1) {
                perso->score += 5; 
                playSoundEffect("asset/sound/piece_etoile.wav", 40);
            } else {
                perso->score += 1; 
                playSoundEffect("asset/sound/coin.wav", 40);
            }
            
            carte->carte[tab_piece->pieces[i].positionY][tab_piece->pieces[i].positionX] = 'M';
        }
    }
}