#include "projet.h"

// Variable globale pour la musique de fond
Mix_Music *backgroundMusic = NULL;

/**
 * Initialise le système audio SDL_mixer
 * @return 1 si l'initialisation réussit, 0 sinon
 */
int initAudio()
{
    // Initialiser le support des formats audio (MP3 en particulier)
    int flags = Mix_Init(MIX_INIT_MP3);
    if ((flags & MIX_INIT_MP3) != MIX_INIT_MP3) {
        printf("Avertissement: SDL_mixer n'a pas pu initialiser le support MP3: %s\n", Mix_GetError());
        // On continue quand même, car on pourrait utiliser d'autres formats
    }
    
    // Initialisation de SDL_mixer avec les paramètres audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Erreur d'initialisation de SDL_mixer: %s\n", Mix_GetError());
        return 0;
    }
    
    printf("Système audio initialisé avec succès\n");
    return 1;
}

/**
 * Charge une musique d'arrière-plan à partir d'un fichier
 * @param musicPath Chemin vers le fichier de musique
 * @return 1 si le chargement réussit, 0 sinon
 */
int loadBackgroundMusic(const char *musicPath)
{
    // Libérer la musique précédente si elle existe
    if (backgroundMusic != NULL)
    {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = NULL;
    }
    
    // Charger le nouveau fichier de musique
    backgroundMusic = Mix_LoadMUS(musicPath);
    if (backgroundMusic == NULL)
    {
        printf("Erreur de chargement de la musique %s: %s\n", musicPath, Mix_GetError());
        return 0;
    }
    
    printf("Musique %s chargée avec succès\n", musicPath);
    return 1;
}

/**
 * Joue la musique d'arrière-plan en boucle
 * @param loops Nombre de répétitions (-1 pour une boucle infinie)
 * @return 1 si la lecture démarre correctement, 0 sinon
 */
int playBackgroundMusic(int loops)
{
    if (backgroundMusic == NULL)
    {
        printf("Erreur: Aucune musique n'a été chargée\n");
        return 0;
    }
    
    if (Mix_PlayMusic(backgroundMusic, loops) == -1)
    {
        printf("Erreur de lecture de la musique: %s\n", Mix_GetError());
        return 0;
    }
    
    return 1;
}

/**
 * Définit le volume de la musique
 * @param volume Valeur du volume (0 à 128)
 */
void setMusicVolume(int volume)
{
    // Vérifier que le volume est dans la plage acceptable
    if (volume < 0)
        volume = 0;
    else if (volume > 128)
        volume = 128;
    
    Mix_VolumeMusic(volume);
}

/**
 * Pause la musique d'arrière-plan
 */
void pauseBackgroundMusic()
{
    Mix_PauseMusic();
}

/**
 * Reprend la lecture de la musique en pause
 */
void resumeBackgroundMusic()
{
    Mix_ResumeMusic();
}

/**
 * Arrête la musique d'arrière-plan
 */
void stopBackgroundMusic()
{
    Mix_HaltMusic();
}

/**
 * Vérifie si la musique est en cours de lecture
 * @return 1 si la musique joue, 0 sinon
 */
int isMusicPlaying()
{
    return Mix_PlayingMusic();
}

/**
 * Charge et joue un effet sonore
 * @param soundPath Chemin vers le fichier audio de l'effet sonore
 * @param volume Volume de l'effet (0-128)
 */
void playSoundEffect(const char *soundPath, int volume)
{
    Mix_Chunk *sound = Mix_LoadWAV(soundPath);
    if (sound == NULL)
    {
        printf("Erreur de chargement de l'effet sonore %s: %s\n", soundPath, Mix_GetError());
        return;
    }
    
    Mix_VolumeChunk(sound, volume);
    int channel = Mix_PlayChannel(-1, sound, 0);
    
    if (channel == -1)
    {
        printf("Erreur de lecture de l'effet sonore: %s\n", Mix_GetError());
        Mix_FreeChunk(sound);
    }
    
    // Note: Nous ne libérons pas automatiquement la mémoire ici
    // Les effets sonores devront être libérés manuellement à la fin du programme
    // ou via une fonction de gestion de la mémoire audio dédiée
}

/**
 * Initialise le système audio et charge la musique du jeu
 * @return 1 si l'initialisation réussit, 0 sinon
 */
int initGameAudio()
{
    if (!initAudio())
    {
        return 0;
    }
    
    // Charger la musique d'arrière-plan du jeu avec le bon nom de fichier
    if (!loadBackgroundMusic("asset/music/ost.mp3"))
    {
        // Essayer un format alternatif si MP3 échoue
        if (!loadBackgroundMusic("asset/music/ost.wav"))
        {
            printf("Impossible de charger la musique d'arrière-plan\n");
            return 0;
        }
    }
    
    // Définir le volume à 75% du maximum
    setMusicVolume(96);
    
    // Jouer la musique en boucle infinie
    if (!playBackgroundMusic(-1))
    {
        return 0;
    }
    
    printf("Musique d'arrière-plan lancée avec succès\n");
    return 1;
}

/**
 * Libère toutes les ressources audio
 */
void cleanupAudio()
{
    // Arrêter la musique si elle est en cours de lecture
    if (Mix_PlayingMusic())
    {
        Mix_HaltMusic();
    }
    
    // Libérer la musique
    if (backgroundMusic != NULL)
    {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = NULL;
    }
    
    // Fermer SDL_mixer
    Mix_CloseAudio();
    Mix_Quit();
    
    printf("Ressources audio libérées\n");
}