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

            // Ajout des nouveaux caractères 'f' et 'o' comme obstacles
            if (contenu_nouv_pos != 'w' && contenu_nouv_pos != ']' && contenu_nouv_pos != '[' 
                && contenu_nouv_pos != 'd' && contenu_nouv_pos != 'f' && contenu_nouv_pos != 'o')
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

void verifier_collision(Carte *carte, Personnage *perso, Tab_gumba *tab_gumba, Tab_plante *tab_plante, const char *fichierTemp, SDL_Window *window, SDL_Renderer *renderer, int niveauActuel)
{
    char caractere_dessous = (perso->positionY + 1 < carte->hauteur) ? carte->carte[perso->positionY + 1][perso->positionX] : 'w';

    // Ajout de 'f' et 'o' comme surfaces solides
    if (caractere_dessous == 'w' || caractere_dessous == ']' || caractere_dessous == '[' || caractere_dessous == 'd' 
        || caractere_dessous == 'f' || caractere_dessous == 'o')
    {
        perso->en_chute = 0;
    }
    else
    {
        perso->en_chute = 1;
    }

    char caractere_dessus = (perso->positionY - 1 >= 0) ? carte->carte[perso->positionY - 1][perso->positionX] : 'w';

    // Ajout de 'f' et 'o' comme obstacles au-dessus
    if (caractere_dessus == 'w' || caractere_dessus == ']' || caractere_dessus == '[' || caractere_dessus == 'd' 
        || caractere_dessus == 'f' || caractere_dessus == 'o')
    {
        perso->peut_monter = 0;
    }
    else
    {
        perso->peut_monter = 1;
    }

    char caractere_devant = (perso->positionX + 1 < carte->largeur) ? carte->carte[perso->positionY][perso->positionX + 1] : 'w';

    // Ajout de 'f' et 'o' comme obstacles devant
    if (caractere_devant == 'w' || caractere_devant == ']' || caractere_devant == '[' || perso->positionX == carte->largeur - 3 
        || caractere_devant == 'd' || caractere_devant == 'f' || caractere_devant == 'o')
    {
        perso->peut_avancer = 0;
    }
    else
    {
        perso->peut_avancer = 1;
    }

    char caractere_derriere = (perso->positionX - 1 >= 0) ? carte->carte[perso->positionY][perso->positionX - 1] : 'w';

    // Ajout de 'f' et 'o' comme obstacles derrière
    if (caractere_derriere == 'w' || caractere_derriere == ']' || caractere_derriere == '[' || perso->positionX == 0 
        || caractere_derriere == 'd' || caractere_derriere == 'f' || caractere_derriere == 'o')
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

        sauvegarderProgression(niveauActuel + 1, perso->nom);

        reinitialiserAudioComplet();

        menuVictoire(perso, niveaux, niveauActuel, niveauMaxDebloque);
        return;
    }
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

    if (caractere_devant == 'w' || caractere_devant == ']' || caractere_devant == 'Q' || gumba->positionX == carte->largeur - 3 || caractere_devant == 'd')
    {
        gumba->peut_avancer = 0;
    }
    else
    {
        gumba->peut_avancer = 1;
    }
    char caractere_derriere = (gumba->positionX - 1 >= 0) ? carte->carte[gumba->positionY][gumba->positionX - 1] : 'w';

    if (caractere_derriere == 'w' || caractere_derriere == '[' || caractere_derriere == 'Q' || gumba->positionX == 0 || caractere_derriere == 'd')
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

void deplacer_joueur(Carte *carte, Personnage *perso, int *isMoving, Tab_gumba *tab_gumba, Tab_plante *tab_plante, const char *fichierTemp, SDL_Window *window, SDL_Renderer *renderer, int niveauActuel)
{
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

    verifier_collision(carte, perso, tab_gumba, tab_plante, fichierTemp, window, renderer, niveauActuel);

    if (perso->en_chute && !perso->en_saut)
    {
        int new_y = perso->positionY + 1;
        if (new_y < carte->hauteur)
        {
            // Ajout de 'f' et 'o' comme obstacles à la chute
            if (carte->carte[new_y][perso->positionX] != 'w' && carte->carte[new_y][perso->positionX] != ']' 
                && carte->carte[new_y][perso->positionX] != '[' && carte->carte[new_y][perso->positionX] != 'd' 
                && carte->carte[new_y][perso->positionX] != 'f' && carte->carte[new_y][perso->positionX] != 'o')
            {
                effacer_position(carte, perso);
                perso->positionY = new_y;
                mettre_position(carte, perso);
                verifier_collision(carte, perso, tab_gumba, tab_plante, fichierTemp, window, renderer, niveauActuel);
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
                            // Ajout de 'f' et 'o' comme obstacles au déplacement latéral
                            if (carte->carte[perso->positionY][next_x] != 'w' && carte->carte[perso->positionY][next_x] != ']' 
                                && carte->carte[perso->positionY][next_x] != '[' && carte->carte[perso->positionY][next_x] != 'd' 
                                && carte->carte[perso->positionY][next_x] != 'f' && carte->carte[perso->positionY][next_x] != 'o')
                            {
                                effacer_position(carte, perso);
                                perso->positionX = next_x;
                                mettre_position(carte, perso);
                                verifier_collision(carte, perso, tab_gumba, tab_plante, fichierTemp, window, renderer, niveauActuel);
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