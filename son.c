#include "projet.h"

Mix_Music *backgroundMusic = NULL;

int initAudio()
{
    
    int flags = Mix_Init(MIX_INIT_MP3);
    if ((flags & MIX_INIT_MP3) != MIX_INIT_MP3) {
        printf("Avertissement: SDL_mixer n'a pas pu initialiser le support MP3: %s\n", Mix_GetError());
       
    }
    
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Erreur d'initialisation de SDL_mixer: %s\n", Mix_GetError());
        return 0;
    }
    
    printf("Système audio initialisé avec succès\n");
    return 1;
}

int loadBackgroundMusic(const char *musicPath)
{
    
    if (backgroundMusic != NULL)
    {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = NULL;
    }
    
    
    backgroundMusic = Mix_LoadMUS(musicPath);
    if (backgroundMusic == NULL)
    {
        printf("Erreur de chargement de la musique %s: %s\n", musicPath, Mix_GetError());
        return 0;
    }
    
    printf("Musique %s chargée avec succès\n", musicPath);
    return 1;
}


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

void setMusicVolume(int volume)
{
    
    if (volume < 0)
        volume = 0;
    else if (volume > 128)
        volume = 128;
    
    Mix_VolumeMusic(volume);
}

void pauseBackgroundMusic()
{
    Mix_PauseMusic();
}

void resumeBackgroundMusic()
{
    Mix_ResumeMusic();
}

void stopBackgroundMusic()
{
    Mix_HaltMusic();
}


int isMusicPlaying()
{
    return Mix_PlayingMusic();
}

void playSoundEffect(const char *soundPath, int volume)
{
    printf("Tentative de lecture du son: %s avec volume %d\n", soundPath, volume);
    
    Mix_Chunk *sound = Mix_LoadWAV(soundPath);
    if (sound == NULL)
    {
        printf("Erreur de chargement de l'effet sonore %s: %s\n", soundPath, Mix_GetError());
        return;
    }
    
    printf("Son chargé avec succès\n");
    
    Mix_VolumeChunk(sound, volume);
    int channel = Mix_PlayChannel(-1, sound, 0);
    
    if (channel == -1)
    {
        printf("Erreur de lecture de l'effet sonore: %s\n", Mix_GetError());
        Mix_FreeChunk(sound);
    }
    else
    {
        printf("Son joué sur le canal %d\n", channel);
        
    }
}

int initGameAudio()
{
    cleanupAudio();

    if (!initAudio())
    {
        return 0;
    }

    printf("Système audio réinitialisé avec succès\n");
    return 1;
}

void reinitialiserAudio() {
    
    if (!isMusicPlaying()) {
        if (backgroundMusic != NULL) {
            
            playBackgroundMusic(-1);
        } else {
            
            initGameAudio();
        }
    }
}

void preserveAudioState() {
    
    pauseBackgroundMusic();
}

void cleanupAudio()
{
   
    if (Mix_PlayingMusic())
    {
        Mix_HaltMusic();
    }
    
    
    if (backgroundMusic != NULL)
    {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = NULL;
    }
    
    
    Mix_CloseAudio();
    Mix_Quit();
    
    printf("Ressources audio libérées\n");
}

void reinitialiserAudioComplet()
{
    
    if (Mix_PlayingMusic())
    {
        Mix_HaltMusic();
    }
    
    if (backgroundMusic != NULL)
    {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = NULL;
    }
    
    
    Mix_CloseAudio();
    Mix_Quit();
    
    
    initAudio();
    
    
    if (loadBackgroundMusic("asset/music/overworld.mp3") || 
        loadBackgroundMusic("asset/music/overworld.wav"))
    {
        setMusicVolume(96);
        playBackgroundMusic(-1);
        printf("Système audio réinitialisé avec succès\n");
    }
    else
    {
        printf("Échec de la réinitialisation audio\n");
    }
}

void restoreAudioState() {
    
    if (!Mix_PlayingMusic()) {
        reinitialiserAudioComplet();
    } else {
        
        resumeBackgroundMusic();
    }
}