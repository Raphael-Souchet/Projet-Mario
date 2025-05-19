#ifndef PROJET_H
#define PROJET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include "SDL_image.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#define TILE_SIZE 25
#define MAX_NIVEAUX 5

typedef struct {
    char fichier[100];
    int debloque;
    int termine;
} Niveau;

extern int LARGEUR_MAP;
extern int SPAWN_X;
extern int SPAWN_Y;
extern int MORT_Y;
extern int niveauActuel;
extern int niveauMaxDebloque;
extern Niveau niveaux[MAX_NIVEAUX];
extern char nomJoueurStocke[100];

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
    char nom[100];
    int niveauMaxDebloque; 
    int score;
    int vie;
    int positionX;
    int positionY;
} SauvegardeInfo;
typedef struct {
    int niveauxDebloques[MAX_NIVEAUX];
} Progression;

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

typedef struct {
    SDL_Texture* brick;
    SDL_Texture* terre;
    SDL_Texture* tuyau;
    SDL_Texture* plante;
    SDL_Texture* herbe5;
    SDL_Texture* terre5;
} GameTextures;

typedef struct {
    SDL_Texture* texture;
    int frameCount;  
    int currentFrame; 
    int frameWidth;      
    int frameHeight;     
    Uint32 frameDuration;
    Uint32 lastFrameTime;
} Animation;

typedef struct {
    Animation* idle;     
    Animation* idle_left;  
    Animation* run;          
    Animation* run_left;     
    int facingRight;         
} PlayerAnimations;

typedef struct {
    Animation* animation;
    int positionX;
    int positionY;
    int type;
    int actif;
} Piece;

typedef struct {
    Piece* pieces;
    int count;
    int capacity;
} Tab_piece;

typedef struct {
    int positionX;
    int positionY;
    int actif;
} StarCoin;

typedef struct {
    StarCoin* starCoins;
    int count;
    int capacity;
} Tab_starcoins;

extern BackgroundTexture* globalBackground;
extern GameTextures* gameTextures;
extern PlayerAnimations* playerAnimations;
extern Tab_piece tab_pieces;
extern Animation* coinAnimation;
extern Tab_starcoins tab_starcoins;
extern Animation* starCoinAnimation;
extern TTF_Font* scoreFont;
extern SDL_Color scoreColor;
extern Animation* flagAnimation;
extern Animation* carnivoreAnimation;
extern SDL_Texture* heartTexture; 
extern Animation* slimeAnimation;

Carte* chargerCarteEnMemoire(const char* fichierCarte);
void libererCarte(Carte* carte);
int sauvegarderCarteVersFichier(Carte* carte, const char* fichierCarte);
int lireSauvegardesExistant(SauvegardeInfo *saves, int maxSaves);
void deplacerCurseur(int x, int y);
void initialiser_gumbas(Carte *carte, Tab_gumba *tab_gumba);
void bouger_gumba(Carte* carte, Tab_gumba* tab_gumba);
void initialiserPlante(Carte *carte, Tab_plante *tab_plante);
void bougerPlante(Carte* carte, Tab_plante* tab_plante);
void verifier_collision_gumba(Carte* carte, Gumba* gumba);
void gerer_saut(Carte* carte, Personnage* perso, int direction);
void effacer_position(Carte* carte, Personnage* perso);
void mettre_position(Carte* carte, Personnage* perso);
void nettoyerSDL(SDL_Window *window, SDL_Renderer *renderer);
int copierFichier(const char *source, const char *destination);
void deplacer_joueur(Carte *carte, Personnage *perso, int *isMoving, Tab_gumba *tab_gumba, Tab_plante *tab_plante, const char *fichierTemp, SDL_Window *window, SDL_Renderer *renderer);
void afficherPaysageSDL(Carte *carte, int positionJoueur, SDL_Renderer *renderer, int playerMoving, Personnage *perso);
void menuPrincipal();
void jouer(const char *fichierTemp, Personnage *perso);
void sauvegarderPartie(Personnage* perso, Carte* carte, const char* fichierTemp);
void resetScores();
void afficherScores();
void verifier_collision(Carte *carte, Personnage *perso, Tab_gumba *tab_gumba, Tab_plante *tab_plante, const char *fichierTemp, SDL_Window *window, SDL_Renderer *renderer);
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
void reinitialiserAudio();
void preserveAudioState();
void restoreAudioState();
void reinitialiserAudioComplet();

GameTextures* loadGameTextures(SDL_Renderer *renderer);
void freeGameTextures(GameTextures *textures);

Animation* loadAnimation(SDL_Renderer* renderer, const char* path, int frameCount, int frameWidth, int frameHeight, Uint32 frameDuration);
void updateAnimation(Animation* animation);
void renderAnimation(SDL_Renderer* renderer, Animation* animation, int x, int y, int flipHorizontal);
void freeAnimation(Animation* animation);
PlayerAnimations* loadPlayerAnimations(SDL_Renderer* renderer);
void freePlayerAnimations(PlayerAnimations* animations);
void loadSlimeAnimation(SDL_Renderer *renderer);
void freeSlimeAnimation();

void initialiser_pieces(Carte* carte, Tab_piece* tab_piece);
void animer_pieces(Tab_piece* tab_piece);
void afficher_pieces(SDL_Renderer* renderer, Tab_piece* tab_piece, int positionJoueur, int debutX);
void liberer_pieces(Tab_piece* tab_piece);
void check_collect_piece(Carte* carte, Tab_piece* tab_piece, Personnage* perso);

TTF_Font* initFont();
void afficherScore(SDL_Renderer* renderer, int score, int vies);

void initialiser_starcoins(Carte* carte, Tab_starcoins* tab_starcoins);
void animer_starcoins(Tab_starcoins* tab_starcoins);
void afficher_starcoins(SDL_Renderer* renderer, Tab_starcoins* tab_starcoins, int positionJoueur, int debutX);
void liberer_starcoins(Tab_starcoins* tab_starcoins);
void check_collect_starcoin(Carte* carte, Tab_starcoins* tab_starcoins, Personnage* perso);
void loadCarnivoreAnimation(SDL_Renderer *renderer);
void freeCarnivoreAnimation();
void reinitialiserAnimations();

SDL_Texture* loadHeartTexture(SDL_Renderer *renderer);
void freeHeartTexture();
void afficherVies(SDL_Renderer* renderer, int vies);

void initialiserNiveaux();
int navigationMenu(int selection, int min, int max, int touche, Niveau *niveaux);
void mettreAJourCoordonnees(int *x, int *y, int *yMort);
void menuVictoire(Personnage *perso);
void chargerProgression(Progression* progression);
void sauvegarderProgression(int nouveauNiveauDebloque, const char* nomJoueur);
int afficherOption(int numero, const char *texte, int selection_actuelle);
void afficherMenuPauseEnJeu();
int menuPauseEnJeu();
int lireSauvegarde();

#endif