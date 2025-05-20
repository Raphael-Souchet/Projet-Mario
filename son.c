#include "projet.h"

Mix_Music *backgroundMusic = NULL;

int initAudio()
{
    
    int flags = Mix_Init(MIX_INIT_MP3);
    if ((flags & MIX_INIT_MP3) != MIX_INIT_MP3) {
       
    }
    
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        return 0;
    }
    
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
        return 0;
    }
    
    return 1;
}


int playBackgroundMusic(int loops)
{
    if (backgroundMusic == NULL)
    {
        return 0;
    }
    
    if (Mix_PlayMusic(backgroundMusic, loops) == -1)
    {
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
    
    Mix_Chunk *sound = Mix_LoadWAV(soundPath);
    if (sound == NULL)
    {
        return;
    }
    
    
    Mix_VolumeChunk(sound, volume);
    int channel = Mix_PlayChannel(-1, sound, 0);
    
    if (channel == -1)
    {
        Mix_FreeChunk(sound);
    }
}

int initGameAudio()
{
    cleanupAudio();

    if (!initAudio())
    {
        return 0;
    }

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
    }
}

void restoreAudioState() {
    
    if (!Mix_PlayingMusic()) {
        reinitialiserAudioComplet();
    } else {
        
        resumeBackgroundMusic();
    }
}