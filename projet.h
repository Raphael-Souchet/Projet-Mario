#ifndef PROJET_H
#define PROJET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "SDL_image.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#define TILE_SIZE 25

extern const int LARGEUR_MAP;
extern const int SPAWN_X;
extern const int SPAWN_Y;
extern const int SPAWN_MORT_X;
extern const int SPAWN_MORT_Y;
extern const int MORT_Y;


typedef struct {
    int positionX;
    int positionY;
    char nom[100];
    int en_saut;
    int score;
    int en_chute;
    int peut_monter;
    int peut_avancer;
    int peut_reculer;
    int vie;
    int etape_saut;
    float vitesse_x;   
} Personnage;

typedef struct {
    int positionX;
    int positionY;
    int peut_avancer;
    int peut_reculer;
    int peut_tomber_devant;
    int peut_tomber_derriere;
    int dernier_deplacement;
} Gumba;

typedef struct {
    Gumba* gumbas;
    int count;
    int compteur;
} Tab_gumba;

typedef struct {
    int positionX;
    int positionY;
    int peut_monter;
    int peut_descendre;
    int compteur;
    int etat_tige;  
    int positionY_base; 
} Plante;

typedef struct {
    Plante* plantes;
    int count;
} Tab_plante;

typedef struct {
    char nom[100];
    int score;
    char date[100];
    int vie;
} Sauvegarde;

typedef struct {
    char** carte;
    int hauteur;
    int largeur;
} Carte;

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} BackgroundTexture;

// Nouvelle structure pour gérer les textures du jeu
typedef struct {
    SDL_Texture* brick;     // Texture pour les briques 'w'
    // Ajouter d'autres textures au besoin pour différents éléments
} GameTextures;

// Ajout au fichier projet.h - Nouvelles structures pour l'animation
typedef struct {
    SDL_Texture* texture;
    int frameCount;      // Nombre total de frames
    int currentFrame;    // Frame actuelle
    int frameWidth;      // Largeur d'une frame
    int frameHeight;     // Hauteur d'une frame
    Uint32 frameDuration;// Durée d'une frame en ms
    Uint32 lastFrameTime;// Horodatage de la dernière frame
} Animation;

// Mise à jour de la structure PlayerAnimations pour inclure l'animation de course
typedef struct {
    Animation* idle;           // Animation idle (face à droite)
    Animation* idle_left;      // Animation idle (face à gauche)
    Animation* run;            // Animation de course (face à droite)
    Animation* run_left;       // Animation de course (face à gauche)
    int facingRight;           // Direction du personnage: 1 = droite, 0 = gauche
} PlayerAnimations;

static BackgroundTexture* globalBackground = NULL;
static GameTextures* gameTextures = NULL;
static PlayerAnimations* playerAnimations = NULL;


Carte* chargerCarteEnMemoire(const char* fichierCarte);
void libererCarte(Carte* carte);
int sauvegarderCarteVersFichier(Carte* carte, const char* fichierCarte);
void deplacerCurseur(int x, int y);
void initialiser_gumbas(Carte* carte, Tab_gumba* tab_gumba);
void bouger_gumba(Carte* carte, Tab_gumba* tab_gumba);
void initialiserPlante(Carte* carte, Tab_plante* tab_plante);
void bougerPlante(Carte* carte, Tab_plante* tab_plante);
void verifier_collision_gumba(Carte* carte, Gumba* gumba);
void gerer_saut(Carte* carte, Personnage* perso, int direction);
void effacer_position(Carte* carte, Personnage* perso);
void mettre_position(Carte* carte, Personnage* perso);
void nettoyerSDL(SDL_Window *window, SDL_Renderer *renderer);
int copierFichier(const char *source, const char *destination);
void deplacer_joueur(Carte* carte, Personnage* perso);
void caracterePaysage(char caractereActuel);
void afficherPaysageSDL(Carte *carte, int positionJoueur, SDL_Renderer *renderer);
void menuPrincipal(const char *fichierTemp);
void jouer(const char *fichierTemp, Personnage *perso);
void sauvegarderPartie(Personnage* perso, Carte* carte, const char* fichierTemp);
int chargerPartie(Personnage *perso);
void menuSauvegarde(Personnage* perso, Carte* carte);
void resetScores();
void afficherScores();
void verifier_collision(Carte* carte, Personnage* perso);
void cacherCurseur();
char* creerNomFichierTemp(const char* nomJoueur);
void menu_mort(Personnage *perso, const char *fichierTemp);
void declencher_rebond(Personnage *perso);
int ecraser_gumba(Carte *carte, Tab_gumba *tab_gumba, Personnage *perso);
int collision_avec_gumba(Tab_gumba *tab_gumba, Personnage *perso);
int collision_avec_plante(Tab_plante *tab_plante, Personnage *perso);
void gerer_collisions(Carte *carte, Personnage *perso, Tab_gumba *tab_gumba, Tab_plante *tab_plante);

int initAudio();
int loadBackgroundMusic(const char *musicPath);
int playBackgroundMusic(int loops);
void setMusicVolume(int volume);
void pauseBackgroundMusic();
void resumeBackgroundMusic();
void stopBackgroundMusic();
int isMusicPlaying();
void playSoundEffect(const char *soundPath, int volume);
int initGameAudio();
void cleanupAudio();

// Nouvelles fonctions pour gérer les textures
GameTextures* loadGameTextures(SDL_Renderer *renderer);
void freeGameTextures(GameTextures* textures);


Animation* loadAnimation(SDL_Renderer* renderer, const char* path, int frameCount, int frameWidth, int frameHeight, Uint32 frameDuration);
void updateAnimation(Animation* animation);
void renderAnimation(SDL_Renderer* renderer, Animation* animation, int x, int y, int flipHorizontal);
void freeAnimation(Animation* animation);
Animation* loadAnimation(SDL_Renderer* renderer, const char* path, int frameCount, int frameWidth, int frameHeight, Uint32 frameDuration);
PlayerAnimations* loadPlayerAnimations(SDL_Renderer* renderer);
void freePlayerAnimations(PlayerAnimations* animations);

// Ajout au fichier affichage.c - Fonctions pour gérer l'animation

#endif