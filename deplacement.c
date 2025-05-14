#include "projet.h"

void gerer_saut(Carte *carte, Personnage *perso, int direction)
{
    int hauteur_saut[8] = {-1, -1, -1, -1, 1, 1, 1, 1};
    
    if (perso->etape_saut == 0 && perso->en_saut == 1)
    {
        perso->etape_saut = 1;
        return;
    }
    
    int hauteur_actuelle = hauteur_saut[perso->etape_saut - 1];
    
    int nouv_y = perso->positionY + hauteur_actuelle;
    
    if (nouv_y >= 0 && nouv_y < carte->hauteur)
    {
        if ((hauteur_actuelle < 0 && perso->peut_monter) || 
            (hauteur_actuelle > 0 && !perso->en_chute))
        {
            if (carte->carte[nouv_y][perso->positionX] == 'c')
            {
                perso->score++;
                carte->carte[nouv_y][perso->positionX] = ' ';
            }
            
            if (carte->carte[nouv_y][perso->positionX] != 'w')
            {
                effacer_position(carte, perso);
                perso->positionY = nouv_y;
                mettre_position(carte, perso);
            }
        }
    }
    
    perso->etape_saut++;
    
    if (perso->etape_saut > 8)
    {
        perso->en_saut = 0;
        perso->etape_saut = 0;
    }
}

void verifier_collision(Carte *carte, Personnage *perso)
{
    char caractere_dessous = (perso->positionY + 1 < carte->hauteur) ? carte->carte[perso->positionY + 1][perso->positionX] : 'w';

    if (caractere_dessous == 'w')
    {
        perso->en_chute = 0;
    }
    else
    {
        perso->en_chute = 1;
    }

    char caractere_dessus = (perso->positionY - 1 >= 0) ? carte->carte[perso->positionY - 1][perso->positionX] : 'w';

    if (caractere_dessus == 'w')
    {
        perso->peut_monter = 0;
    }
    else
    {
        perso->peut_monter = 1;
    }

    char caractere_devant = (perso->positionX + 1 < carte->largeur) ? carte->carte[perso->positionY][perso->positionX + 1] : 'w';

    if (caractere_devant == 'w' || perso->positionX == carte->largeur - 3)
    {
        perso->peut_avancer = 0;
    }
    else
    {
        perso->peut_avancer = 1;
    }

    char caractere_derriere = (perso->positionX - 1 >= 0) ? carte->carte[perso->positionY][perso->positionX - 1] : 'w';

    if (caractere_derriere == 'w' || perso->positionX == 0)
    {
        perso->peut_reculer = 0;
    }
    else
    {
        perso->peut_reculer = 1;
    }

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

    if (caractere_devant == 'w' || caractere_devant == ']' || caractere_devant == 'Q' || gumba->positionX == carte->largeur - 3)
    {
        gumba->peut_avancer = 0;
    }
    else
    {
        gumba->peut_avancer = 1;
    }
    char caractere_derriere = (gumba->positionX - 1 >= 0) ? carte->carte[gumba->positionY][gumba->positionX - 1] : 'w';

    if (caractere_derriere == 'w' || caractere_derriere == '[' || caractere_derriere == 'Q' || gumba->positionX == 0)
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

void deplacer_joueur(Carte *carte, Personnage *perso, int *isMoving) {
    if (perso->positionY >= MORT_Y) {
        *isMoving = 0;
        return;
    }

    int input_deplacement = 0;
    const int VITESSE_FIXE = 1; // Vitesse fixe pour des mouvements plus directs
    
    *isMoving = 0;

    if (GetAsyncKeyState('D') & 0x8000) {
        input_deplacement = 1;
        *isMoving = 1;
        if (playerAnimations != NULL) {
            playerAnimations->facingRight = 1;
        }
    }
    else if (GetAsyncKeyState('Q') & 0x8000) {
        input_deplacement = -1;
        *isMoving = 1;
        if (playerAnimations != NULL) {
            playerAnimations->facingRight = 0;
        }
    }

    // Application directe de la vitesse sans accélération ni décélération
    if (input_deplacement != 0) {
        perso->vitesse_x = input_deplacement * VITESSE_FIXE;
        *isMoving = 1;
    } else {
        perso->vitesse_x = 0; // Arrêt immédiat quand aucune touche n'est pressée
    }

    verifier_collision(carte, perso);

    if (perso->en_chute && !perso->en_saut) {
        int new_y = perso->positionY + 1;
        if (new_y < carte->hauteur) {
            if (carte->carte[new_y][perso->positionX] == 'c') {
                perso->score++;
                carte->carte[new_y][perso->positionX] = ' ';
                playSoundEffect("asset/sound/coin.wav", 64);
            }
            
            if (carte->carte[new_y][perso->positionX] != 'w') {
                effacer_position(carte, perso);
                perso->positionY = new_y;
                mettre_position(carte, perso);
                verifier_collision(carte, perso);
                *isMoving = 1; 
            }
        }
    }

    if (perso->vitesse_x != 0) {
        int deplacement_x = (perso->vitesse_x > 0) ? VITESSE_FIXE : -VITESSE_FIXE;
        
        if (deplacement_x != 0) {
            int new_x = perso->positionX + deplacement_x;
            
            if (new_x >= 0 && new_x < carte->largeur) {
                if ((deplacement_x > 0 && !perso->peut_avancer) ||
                    (deplacement_x < 0 && !perso->peut_reculer)) {
                    perso->vitesse_x = 0;
                } else {
                    int dir = (deplacement_x > 0) ? 1 : -1;
                    for (int i = 0; i < abs(deplacement_x); i++) {
                        int next_x = perso->positionX + dir;
                        
                        if (next_x >= 0 && next_x < carte->largeur) {
                            if (carte->carte[perso->positionY][next_x] == 'c') {
                                perso->score++;
                                carte->carte[perso->positionY][next_x] = ' ';
                                playSoundEffect("asset/sound/coin.wav", 64);
                            }
                            
                            if (carte->carte[perso->positionY][next_x] != 'w') {
                                effacer_position(carte, perso);
                                perso->positionX = next_x;
                                mettre_position(carte, perso);
                                verifier_collision(carte, perso);
                                *isMoving = 1;
                            } else {
                                perso->vitesse_x = 0;
                                break;
                            }
                        } else {
                            perso->vitesse_x = 0;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (perso->en_saut)
    {
        gerer_saut(carte, perso, input_deplacement);
        *isMoving = 1; 
    }
    else if ((GetAsyncKeyState('Z') & 0x8000) && !perso->en_chute) {
        perso->en_saut = 1;
        perso->etape_saut = 0;
        gerer_saut(carte, perso, input_deplacement);
        *isMoving = 1;
        playSoundEffect("asset/sound/jump.wav", 64);
    }
}

void jouer(const char *fichierTemp, Personnage *perso)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return;
    }

    
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return;
    }

    if (!initGameAudio())
    {
        printf("Avertissement: L'audio n'a pas pu être initialisé. Le jeu continuera sans son.\n");
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
        cleanupAudio();
        IMG_Quit();
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        cleanupAudio();  
        IMG_Quit();
        SDL_Quit();
        return;
    }

    Carte *carte = chargerCarteEnMemoire(fichierTemp);
    if (carte == NULL)
    {
        printf("Erreur: Impossible de charger la carte %s\n", fichierTemp);
        nettoyerSDL(window, renderer);
        cleanupAudio();
        SDL_Quit();
        return;
    }

    if (perso->positionX < 0 || perso->positionX >= carte->largeur ||
        perso->positionY < 0 || perso->positionY >= carte->hauteur)
    {
        perso->positionX = SPAWN_X;
        perso->positionY = SPAWN_Y;
    }

    perso->vitesse_x = 0.0f;
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
    
    int playerMoving = 0;

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

        Uint32 currentTime = SDL_GetTicks();
        int deltaTime = currentTime - lastTime;

        if (deltaTime >= frameDelay)
        {
            lastTime = currentTime;

            bouger_gumba(carte, &tab_gumba);
            bougerPlante(carte, &tab_plante);

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            {
                while (_kbhit())
                    _getch();
                
                sauvegarderCarteVersFichier(carte, fichierTemp);

                stopBackgroundMusic();
                cleanupAudio();

                nettoyerSDL(window, renderer);
                IMG_Quit();

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

                menuSauvegarde(perso, carte);
                return;
            }

            deplacer_joueur(carte, perso, &playerMoving);

            gerer_collisions(carte, perso, &tab_gumba, &tab_plante);

            afficherPaysageSDL(carte, perso->positionX, renderer, playerMoving);
            SDL_RenderPresent(renderer);

            if (perso->positionY >= MORT_Y)
            {
                perso->vie--;
                sauvegarderCarteVersFichier(carte, fichierTemp);

                stopBackgroundMusic();
                cleanupAudio();

                nettoyerSDL(window, renderer);
                IMG_Quit();

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

    stopBackgroundMusic();
    cleanupAudio();

    nettoyerSDL(window, renderer);
    IMG_Quit();

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
}