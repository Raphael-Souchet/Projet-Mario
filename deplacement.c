#include "projet.h"

// Structure pour les textures du jeu
typedef struct {
    SDL_Texture* texture_player_idle;  // Sprite fixe du joueur (tile_0000)
    SDL_Texture* texture_player_move1; // Premier sprite d'animation (tile_0000)
    SDL_Texture* texture_player_move2; // Deuxième sprite d'animation (tile_0001)
    SDL_Texture* texture_grass;        // Texture pour l'herbe (tile_0000)
    SDL_Texture* texture_block;        // Texture pour les blocs (tile_0006)
    int animation_frame;               // Compteur pour alterner les frames
    int est_en_mouvement;              // 1 si en mouvement, 0 sinon
    int direction;                     // 1 pour droite, -1 pour gauche
} GameTextures;

// Variable globale pour les textures du jeu
GameTextures gameTextures = {NULL, NULL, NULL, NULL, NULL, 0, 0, 1};

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

void caracterePaysage(char caractereActuel)
{
    switch (caractereActuel)
    {
    case 'w':
    case 'b':
    case 'd':
        printf("#");
        break;
    case 'c':
        printf("o");
        break;
    case 'M':
        printf("&");
        break;
    case 'Q':
        printf("@");
        break;
    case 'u':
        printf("u");
        break;
    case ']':
        printf("]");
        break;
    case '[':
        printf("[");
        break;
    case '|':
        printf("|");
        break;
    default:
        printf("%c", caractereActuel);
        break;
    }
}

// Fonction pour charger toutes les textures du jeu
void chargerTextures(SDL_Renderer *renderer) {
    // Base du chemin vers les assets
    const char* base_path = "C:\\Users\\duber\\cours L1\\programation\\mario_vscode\\Projet-Mario\\asset\\sprit\\";
    
    // Chemins vers les sprites
    const char* chemin_player1 = "C:\\Users\\duber\\cours L1\\programation\\mario_vscode\\Projet-Mario\\asset\\Tiles\\Characters\\tile_0000.png";
    const char* chemin_player2 = "C:\\Users\\duber\\cours L1\\programation\\mario_vscode\\Projet-Mario\\asset\\Tiles\\Characters\\tile_0001.png";
    const char* chemin_grass = "C:\\Users\\duber\\cours L1\\programation\\mario_vscode\\Projet-Mario\\asset\\Tiles\\tile_0000.png";
    const char* chemin_block = "C:\\Users\\duber\\cours L1\\programation\\mario_vscode\\Projet-Mario\\asset\\Tiles\\tile_0006.png";
    
    // Initialiser SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur lors de l'initialisation de SDL_image: %s\n", IMG_GetError());
        return;
    }
    
    // Charger les images PNG
    SDL_Surface* surface_player1 = IMG_Load(chemin_player1);
    SDL_Surface* surface_player2 = IMG_Load(chemin_player2);
    SDL_Surface* surface_grass = IMG_Load(chemin_grass);
    SDL_Surface* surface_block = IMG_Load(chemin_block);
    
    if (surface_player1 == NULL) {
        printf("Erreur lors du chargement de l'image %s: %s\n", chemin_player1, IMG_GetError());
    }
    
    if (surface_player2 == NULL) {
        printf("Erreur lors du chargement de l'image %s: %s\n", chemin_player2, IMG_GetError());
    }
    
    if (surface_grass == NULL) {
        printf("Erreur lors du chargement de l'image %s: %s\n", chemin_grass, IMG_GetError());
    }
    
    if (surface_block == NULL) {
        printf("Erreur lors du chargement de l'image %s: %s\n", chemin_block, IMG_GetError());
    }
    
    if (surface_player1 == NULL || surface_player2 == NULL || 
        surface_grass == NULL || surface_block == NULL) {
        return;
    }
    
    // Créer les textures à partir des surfaces
    gameTextures.texture_player_idle = SDL_CreateTextureFromSurface(renderer, surface_player1);
    gameTextures.texture_player_move1 = SDL_CreateTextureFromSurface(renderer, surface_player1);
    gameTextures.texture_player_move2 = SDL_CreateTextureFromSurface(renderer, surface_player2);
    gameTextures.texture_grass = SDL_CreateTextureFromSurface(renderer, surface_grass);
    gameTextures.texture_block = SDL_CreateTextureFromSurface(renderer, surface_block);
    
    // Libérer les surfaces
    SDL_FreeSurface(surface_player1);
    SDL_FreeSurface(surface_player2);
    SDL_FreeSurface(surface_grass);
    SDL_FreeSurface(surface_block);
    
    // Initialiser les variables d'animation
    gameTextures.animation_frame = 0;
    gameTextures.est_en_mouvement = 0;
    gameTextures.direction = 1; // Par défaut, le personnage regarde à droite
}

// Fonction pour libérer les textures
void libererTextures() {
    if (gameTextures.texture_player_idle != NULL) {
        SDL_DestroyTexture(gameTextures.texture_player_idle);
        gameTextures.texture_player_idle = NULL;
    }
    if (gameTextures.texture_player_move1 != NULL) {
        SDL_DestroyTexture(gameTextures.texture_player_move1);
        gameTextures.texture_player_move1 = NULL;
    }
    if (gameTextures.texture_player_move2 != NULL) {
        SDL_DestroyTexture(gameTextures.texture_player_move2);
        gameTextures.texture_player_move2 = NULL;
    }
    if (gameTextures.texture_grass != NULL) {
        SDL_DestroyTexture(gameTextures.texture_grass);
        gameTextures.texture_grass = NULL;
    }
    if (gameTextures.texture_block != NULL) {
        SDL_DestroyTexture(gameTextures.texture_block);
        gameTextures.texture_block = NULL;
    }
}

void afficherPaysageSDL(Carte *carte, int positionJoueur, SDL_Renderer *renderer)
{
    int largeurAffichage = 50;
    int demiLargeur = largeurAffichage / 2;
    
    int debutX = positionJoueur - demiLargeur;
    int finX = positionJoueur + demiLargeur;

    if (debutX < 0) debutX = 0;
    if (finX > carte->largeur) finX = carte->largeur;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

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

            switch(carte->carte[y][x]) {
        case 'w': 
            SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); 
            break;
        case 'c': 
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); 
            break;
        case 'M':
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
            break;
        case 'Q': 
            SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255); 
            break;
        case 'u':
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
            break;
        case ']':
        case '[':
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
            break;
        case '|':
            SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); 
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); 
            break;
    }
    
    SDL_RenderFillRect(renderer, &tile);
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

void gerer_saut(Carte *carte, Personnage *perso, int direction)
{
    int hauteur_saut[8] = {-1, -1, -1, -1, 1, 1, 1, 1};

    SDL_Renderer *renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));

    for (int i = 0; i < 8; i++)
    {
        int nouv_x = perso->positionX + direction;
        int nouv_y = perso->positionY + hauteur_saut[i];

        if (nouv_x < 0 || nouv_x >= carte->largeur || nouv_y < 0 || nouv_y >= carte->hauteur)
        {
            continue;
        }

        if (carte->carte[nouv_y][nouv_x] == 'c')
        {
            perso->score++;
            carte->carte[nouv_y][nouv_x] = ' ';
        }

        char caractere_destination = carte->carte[nouv_y][nouv_x];
        char caractere_lateral = carte->carte[perso->positionY][nouv_x];

        if (nouv_x >= 0 && nouv_x < carte->largeur && nouv_y >= 0 &&
            caractere_destination != 'w' && caractere_lateral != 'w' && perso->peut_monter)
        {
            effacer_position(carte, perso);
            perso->positionX = nouv_x;
            perso->positionY = nouv_y;
            mettre_position(carte, perso);

            if (renderer != NULL)
            {
                afficherPaysageSDL(carte, perso->positionX, renderer);
                SDL_RenderPresent(renderer);
            }

            Sleep(85);
        }
    }

    perso->en_saut = 0;
}

void verifier_collision(Carte *carte, Personnage *perso)
{
    char caractere_dessous = (perso->positionY + 1 < carte->hauteur) ? carte->carte[perso->positionY + 1][perso->positionX] : 'w';
    
    // Gestion de la chute 
    if (est_bloc_solide(caractere_dessous)) {
        perso->en_chute = 0;
    } else {
        perso->en_chute = 1;
    }
    
    // Vérification du caractère au-dessus du personnage
    char caractere_dessus = (perso->positionY - 1 >= 0) ? carte->carte[perso->positionY - 1][perso->positionX] : 'w';
    
    // Vérification de la possibilité de monter
    if (est_bloc_solide(caractere_dessus)) {
        perso->peut_monter = 0;
    } else {
        perso->peut_monter = 1;
    }
    
    // Vérification du caractère devant le personnage
    char caractere_devant = (perso->positionX + 1 < carte->largeur) ? carte->carte[perso->positionY][perso->positionX + 1] : 'w';
    
    // Vérification de la possibilité d'avancer
    if (est_bloc_solide(caractere_devant) || perso->positionX == carte->largeur - 3) {
        perso->peut_avancer = 0;
    } else {
        perso->peut_avancer = 1;
    }
    
    // Vérification du caractère derrière le personnage
    char caractere_derriere = (perso->positionX - 1 >= 0) ? carte->carte[perso->positionY][perso->positionX - 1] : 'w';
    
    // Vérification de la possibilité de reculer
    if (est_bloc_solide(caractere_derriere) || perso->positionX == 0) {
        perso->peut_reculer = 0;
    } else {
        perso->peut_reculer = 1;
    }
    
    // Vérifications des limites pour garder le personnage dans la carte
    if (perso->positionX < 0)
        perso->positionX = 0;
    if (perso->positionX >= carte->largeur)
        perso->positionX = carte->largeur - 1;
    if (perso->positionY < 0)
        perso->positionY = 0;
}

void verifier_collision_gumba(Carte *carte, Gumba *gumba)
{
    char caractere_devant_dessous = (gumba->positionY + 1 < carte->hauteur && gumba->positionX + 1 < carte->largeur) ? carte->carte[gumba->positionY + 1][gumba->positionX + 1] : 'w';

    if (caractere_devant_dessous == ' ')
    {
        gumba->peut_tomber_devant = 1;
    }
    else
    {
        gumba->peut_tomber_devant = 0;
    }

    char caractere_derriere_dessous = (gumba->positionY + 1 < carte->hauteur && gumba->positionX - 1 >= 0) ? carte->carte[gumba->positionY + 1][gumba->positionX - 1] : 'w';

    if (caractere_derriere_dessous == ' ')
    {
        gumba->peut_tomber_derriere = 1;
    }
    else
    {
        gumba->peut_tomber_derriere = 0;
    }

    char caractere_devant = (gumba->positionX + 1 < carte->largeur) ? carte->carte[gumba->positionY][gumba->positionX + 1] : 'w';

    if (est_bloc_solide(caractere_devant) || caractere_devant == ']' || caractere_devant == 'Q' || gumba->positionX == carte->largeur - 3)
    {
        gumba->peut_avancer = 0;
    }
    else
    {
        gumba->peut_avancer = 1;
    }
    char caractere_derriere = (gumba->positionX - 1 >= 0) ? carte->carte[gumba->positionY][gumba->positionX - 1] : 'w';

    if (est_bloc_solide(caractere_derriere) || caractere_derriere == '[' || caractere_derriere == 'Q' || gumba->positionX == 0)
    {
        gumba->peut_reculer = 0;
    }
    else
    {
        gumba->peut_reculer = 1;
    }

    if (gumba->positionX < 0)
        gumba->positionX = 0;
    if (gumba->positionX >= carte->largeur)
        gumba->positionX = carte->largeur - 1;
    if (gumba->positionY < 0)
        gumba->positionY = 0;
}

void nettoyerSDL(SDL_Window *window, SDL_Renderer *renderer)
{
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

void gerer_saut(Carte *carte, Personnage *perso, int direction)
{
    int hauteur_saut[8] = {-1, -1, -1, -1, 1, 1, 1, 1};

    SDL_Renderer *renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));

    for (int i = 0; i < 8; i++)
    {
        int nouv_x = perso->positionX + direction;
        int nouv_y = perso->positionY + hauteur_saut[i];

        if (nouv_x < 0 || nouv_x >= carte->largeur || nouv_y < 0 || nouv_y >= carte->hauteur)
        {
            continue;
        }

        if (carte->carte[nouv_y][nouv_x] == 'c')
        {
            perso->score++;
            carte->carte[nouv_y][nouv_x] = ' ';
        }

        char caractere_destination = carte->carte[nouv_y][nouv_x];
        char caractere_lateral = carte->carte[perso->positionY][nouv_x];

        if (nouv_x >= 0 && nouv_x < carte->largeur && nouv_y >= 0 &&
            !est_bloc_solide(caractere_destination) && !est_bloc_solide(caractere_lateral) && perso->peut_monter)
        {
            effacer_position(carte, perso);
            perso->positionX = nouv_x;
            perso->positionY = nouv_y;
            mettre_position(carte, perso);

            if (renderer != NULL)
            {
                afficherPaysageSDL(carte, perso->positionX, renderer);
                SDL_RenderPresent(renderer);
            }

            Sleep(85);
        }
    }

    perso->en_saut = 0;
}

void deplacer_joueur(Carte *carte, Personnage *perso)
{
    if (perso->positionY >= MORT_Y)
    {
        return;
    }

    int deplacement_x = 0;
    int ancien_x = perso->positionX;
    int ancien_y = perso->positionY;

    if (GetAsyncKeyState('D') & 0x8000) {
        deplacement_x = 1;
        gameTextures.direction = -1;  // Regarder à droite
    }
    if (GetAsyncKeyState('Q') & 0x8000) {
        deplacement_x = -1;
        gameTextures.direction = 1;  // Regarder à gauche
    }

    verifier_collision(carte, perso);

    if (perso->en_chute && !perso->en_saut)
    {
        int new_y = perso->positionY + 1;
        if (new_y < carte->hauteur)
        {
            if (carte->carte[new_y][perso->positionX] == 'c')
            {
                perso->score++;
                carte->carte[new_y][perso->positionX] = ' ';
            }
            
            if (carte->carte[new_y][perso->positionX] != 'w')
            {
                effacer_position(carte, perso);
                perso->positionY = new_y;
                mettre_position(carte, perso);
                verifier_collision(carte, perso);
            }
        }
    }

    if (deplacement_x != 0)
    {
        int new_x = perso->positionX + deplacement_x;
        if (new_x >= 0 && new_x < carte->largeur)
        {
            if (carte->carte[perso->positionY][new_x] == 'c')
            {
                perso->score++;
                carte->carte[perso->positionY][new_x] = ' ';
            }
            
            if ((deplacement_x > 0 && perso->peut_avancer) ||
                (deplacement_x < 0 && perso->peut_reculer))
            {
                if (carte->carte[perso->positionY][new_x] != 'w')
                {
                    effacer_position(carte, perso);
                    perso->positionX = new_x;
                    mettre_position(carte, perso);
                    verifier_collision(carte, perso);
                }
            }
        }
    }

    // Mettre à jour l'état de mouvement pour l'animation
    if (ancien_x != perso->positionX || ancien_y != perso->positionY) {
        gameTextures.est_en_mouvement = 1;
    } else {
        gameTextures.est_en_mouvement = 0;
    }

    if (perso->en_saut)
    {
        gerer_saut(carte, perso, deplacement_x);
    }
    else if ((GetAsyncKeyState('Z') & 0x8000) && !perso->en_chute)
    {
        perso->en_saut = 1;
        perso->etape_saut = 0;
        gerer_saut(carte, perso, deplacement_x);
    }
}

// Mise à jour de la fonction jouer pour utiliser les nouvelles textures
void jouer(const char *fichierTemp, Personnage *perso)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return;
    }

    // Initialiser SDL_image pour les PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return;
    }

    int largeurAffichage = 50;
    SDL_Window *window = SDL_CreateWindow(
        "Super Mario",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        largeurAffichage * TILE_SIZE,
        20 * TILE_SIZE,
        0);

    if (window == NULL)
    {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Carte *carte = chargerCarteEnMemoire(fichierTemp);
    if (carte == NULL)
    {
        printf("Erreur: Impossible de charger la carte %s\n", fichierTemp);
        nettoyerSDL(window, renderer);
        Sleep(1500);
        return;
    }

    if (perso->positionX < 0 || perso->positionX >= carte->largeur ||
        perso->positionY < 0 || perso->positionY >= carte->hauteur)
    {
        perso->positionX = SPAWN_X;
        perso->positionY = SPAWN_Y;
    }

    carte->carte[perso->positionY][perso->positionX] = 'M';

    Tab_gumba tab_gumba = {NULL, 0};
    Tab_plante tab_plante = {NULL, 0};
    initialiserPlante(carte, &tab_plante);
    initialiser_gumbas(carte, &tab_gumba);

    cacherCurseur();
    
    int quit = 0;
    Uint32 lastTime = SDL_GetTicks();
    const int FPS = 22;
    const int frameDelay = 1500 / FPS;

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

        Sleep(60);
        bouger_gumba(carte, &tab_gumba);
        
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            while (_kbhit()) _getch();
            sauvegarderCarteVersFichier(carte, fichierTemp);
            
            nettoyerSDL(window, renderer);
            
            if (tab_gumba.gumbas != NULL) {
                free(tab_gumba.gumbas);
                tab_gumba.gumbas = NULL;
            }
            
            menuSauvegarde(perso, carte);
            return;
        }
        
        deplacer_joueur(carte, perso);

        afficherPaysageSDL(carte, perso->positionX, renderer);  
        SDL_RenderPresent(renderer);

            if (perso->positionY >= MORT_Y)
            {
                perso->vie--;
                sauvegarderCarteVersFichier(carte, fichierTemp);

            nettoyerSDL(window, renderer);

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

    // Libérer les textures avant de nettoyer SDL
    libererTextures();
    nettoyerSDL(window, renderer);

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
    // À la fin de la fonction, n'oubliez pas de quitter SDL_image aussi
    IMG_Quit();  // Quitter SDL_image
}