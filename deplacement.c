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
        if ((hauteur_actuelle < 0 && perso->peut_monter) || (hauteur_actuelle > 0))
        {
            char contenu_nouv_pos = carte->carte[nouv_y][perso->positionX];

            // Si la position est traversable (n'est PAS un bloc solide)
            if (strchr("w]n[dfo|yegz", contenu_nouv_pos) == NULL)
            {
                effacer_position(carte, perso);
                perso->positionY = nouv_y;
                mettre_position(carte, perso);
            }
            else if (hauteur_actuelle < 0)
            {
                perso->etape_saut = 5;
                return;
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

void verifier_collision(Carte *carte, Personnage *perso, Tab_gumba *tab_gumba, Tab_plante *tab_plante, const char *fichierTemp, SDL_Window *window, SDL_Renderer *renderer)
{
    // Liste des blocs solides
    const char* solid_chars = "w]n[dfo|yegz";

    char caractere_dessous = (perso->positionY + 1 < carte->hauteur) ? carte->carte[perso->positionY + 1][perso->positionX] : 'w';

    // Si la case sous le personnage est un bloc solide
    if (strchr(solid_chars, caractere_dessous) != NULL)
    {
        perso->en_chute = 0;
    }
    else
    {
        perso->en_chute = 1;
    }

    char caractere_dessus = (perso->positionY - 1 >= 0) ? carte->carte[perso->positionY - 1][perso->positionX] : 'w';

    // Si la case au-dessus du personnage est un bloc solide
    if (strchr(solid_chars, caractere_dessus) != NULL)
    {
        perso->peut_monter = 0;
    }
    else
    {
        perso->peut_monter = 1;
    }

    char caractere_devant = (perso->positionX + 1 < carte->largeur) ? carte->carte[perso->positionY][perso->positionX + 1] : 'w';

    // Si la case devant le personnage N'est PAS un bloc solide 
    // (c'est une erreur, devrait être != NULL pour vérifier s'il peut avancer)
    if (strchr(solid_chars, caractere_devant) != NULL)
    {
        perso->peut_avancer = 0;
    }
    else
    {
        perso->peut_avancer = 1;
    }

    char caractere_derriere = (perso->positionX - 1 >= 0) ? carte->carte[perso->positionY][perso->positionX - 1] : 'w';

    // Si la case derrière le personnage N'est PAS un bloc solide
    // (c'est une erreur, devrait être != NULL pour vérifier s'il peut reculer)
    if (strchr(solid_chars, caractere_derriere) != NULL)
    {
        perso->peut_reculer = 0;
    }
    else
    {
        perso->peut_reculer = 1;
    }

    if (perso->positionX < 0)
        perso->positionX = 0;
    if (perso->positionX > carte->largeur - 5)
    {
        preserveAudioState();
        nettoyerSDL(window, renderer);
        libererCarte(carte);

        extern int niveauActuel;
        sauvegarderProgression(niveauActuel + 1, perso->nom, perso->score);
        extern int niveauMaxDebloque;
        extern Niveau niveaux[MAX_NIVEAUX];
        menuVictoire(perso);
        return;
    }
    if (perso->positionY < 0)
        perso->positionY = 0;
}

void verifier_collision_gumba(Carte *carte, Gumba *gumba)
{
    // Liste des caractères solides
    const char* solid_chars = "w]n[dfo|yegz";

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

    // Vérifie si la case devant le gumba contient un bloc solide
    if (strchr(solid_chars, caractere_devant) != NULL)
    {
        gumba->peut_avancer = 0;
    }
    else
    {
        gumba->peut_avancer = 1;
    }
    
    char caractere_derriere = (gumba->positionX - 1 >= 0) ? carte->carte[gumba->positionY][gumba->positionX - 1] : 'w';

    // Vérifie si la case derrière le gumba contient un bloc solide
    if (strchr(solid_chars, caractere_derriere) != NULL)
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

void deplacer_joueur(Carte *carte, Personnage *perso, int *isMoving, Tab_gumba *tab_gumba, Tab_plante *tab_plante, const char *fichierTemp, SDL_Window *window, SDL_Renderer *renderer)
{
    extern int MORT_Y;
    if (perso->positionY >= MORT_Y)
    {
        *isMoving = 0;
        return;
    }

    int input_deplacement = 0;
    const int VITESSE_FIXE = 1;

    *isMoving = 0;

    if (GetAsyncKeyState('D') & 0x8000)
    {
        input_deplacement = 1;
        *isMoving = 1;

        if (playerAnimations != NULL)
        {
            playerAnimations->facingRight = 1;
        }
    }
    else if (GetAsyncKeyState('Q') & 0x8000)
    {
        input_deplacement = -1;
        *isMoving = 1;
        if (playerAnimations != NULL)
        {
            playerAnimations->facingRight = 0;
        }
    }

    if (input_deplacement != 0)
    {
        perso->vitesse_x = input_deplacement * VITESSE_FIXE;
        *isMoving = 1;
    }
    else
    {
        perso->vitesse_x = 0;
    }
    extern int niveauActuel;
    verifier_collision(carte, perso, tab_gumba, tab_plante, fichierTemp, window, renderer);

    if (perso->en_chute && !perso->en_saut)
    {
        int new_y = perso->positionY + 1;
        if (new_y < carte->hauteur)
        {
            // La logique est inversée ici - on veut pouvoir tomber si la case sous nous N'est PAS un bloc solide
            char caractere_dessous = carte->carte[new_y][perso->positionX];
            if (strchr("w]n[dfo|yegz", caractere_dessous) == NULL)
            {
                effacer_position(carte, perso);
                perso->positionY = new_y;
                mettre_position(carte, perso);
                verifier_collision(carte, perso, tab_gumba, tab_plante, fichierTemp, window, renderer);
                *isMoving = 1;
            }
        }
    }

    if (perso->vitesse_x != 0)
    {
        int deplacement_x = (perso->vitesse_x > 0) ? VITESSE_FIXE : -VITESSE_FIXE;

        if (playerAnimations != NULL)
        {
            if (deplacement_x > 0)
            {
                playerAnimations->facingRight = 1;
            }
            else if (deplacement_x < 0)
            {
                playerAnimations->facingRight = 0;
            }
        }

        if (deplacement_x != 0)
        {
            int new_x = perso->positionX + deplacement_x;

            if (new_x >= 0 && new_x < carte->largeur)
            {
                if ((deplacement_x > 0 && !perso->peut_avancer) ||
                    (deplacement_x < 0 && !perso->peut_reculer))
                {
                    perso->vitesse_x = 0;
                }
                else
                {
                    int dir = (deplacement_x > 0) ? 1 : -1;
                    for (int i = 0; i < abs(deplacement_x); i++)
                    {
                        int next_x = perso->positionX + dir;

                        if (next_x >= 0 && next_x < carte->largeur)
                        {
                            // La logique est inversée ici - on veut pouvoir se déplacer si la case suivante N'est PAS un bloc solide
                            char caractere_suivant = carte->carte[perso->positionY][next_x];
                            if (strchr("w]n[dfo|yegz", caractere_suivant) == NULL)
                            {
                                effacer_position(carte, perso);
                                perso->positionX = next_x;
                                mettre_position(carte, perso);
                                verifier_collision(carte, perso, tab_gumba, tab_plante, fichierTemp, window, renderer);
                                *isMoving = 1;
                            }
                            else
                            {
                                perso->vitesse_x = 0;
                                break;
                            }
                        }
                        else
                        {
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
    else if ((GetAsyncKeyState('Z') & 0x8000) && !perso->en_chute)
    {
        perso->en_saut = 1;
        perso->etape_saut = 0;
        gerer_saut(carte, perso, input_deplacement);
        *isMoving = 1;
        playSoundEffect("asset/sound/jump.wav", 40);
    }
}