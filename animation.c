#include "projet.h"

Animation *loadAnimation(SDL_Renderer *renderer, const char *path, int frameCount, int frameWidth, int frameHeight, Uint32 frameDuration)
{
    Animation *animation = (Animation *)malloc(sizeof(Animation));
    if (animation == NULL)
    {
        return NULL;
    }

    animation->texture = NULL;
    animation->frameCount = frameCount;
    animation->currentFrame = 0;
    animation->frameWidth = frameWidth;
    animation->frameHeight = frameHeight;
    animation->frameDuration = frameDuration;
    animation->lastFrameTime = SDL_GetTicks();

    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL)
    {
        free(animation);
        return NULL;
    }

    animation->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (animation->texture == NULL)
    {
        SDL_FreeSurface(surface);
        free(animation);
        return NULL;
    }

    SDL_FreeSurface(surface);

    return animation;
}

void updateAnimation(Animation *animation)
{
    if (animation == NULL)
        return;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - animation->lastFrameTime > animation->frameDuration)
    {
        animation->currentFrame = (animation->currentFrame + 1) % animation->frameCount;
        animation->lastFrameTime = currentTime;
    }
}

void renderAnimation(SDL_Renderer *renderer, Animation *animation, int x, int y, int flipHorizontal)
{
    if (animation == NULL || renderer == NULL || animation->texture == NULL)
    {
        return;
    }

    SDL_Rect srcRect = {
        animation->currentFrame * animation->frameWidth,
        0,
        animation->frameWidth,
        animation->frameHeight};

    float scaleFactor = 1.0f;

    if (animation == coinAnimation)
    {
        scaleFactor = 1.5f;
    }

    int finalWidth = (int)(animation->frameWidth * scaleFactor);
    int finalHeight = (int)(animation->frameHeight * scaleFactor);

    SDL_Rect dstRect = {
        x - finalWidth / 2, 
        y - finalHeight,  
        finalWidth,
        finalHeight};

    SDL_RendererFlip flip = flipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    SDL_RenderCopyEx(renderer, animation->texture, &srcRect, &dstRect, 0, NULL, flip);
}

void freeAnimation(Animation *animation)
{
    if (animation != NULL)
    {
        if (animation->texture != NULL)
        {
            SDL_DestroyTexture(animation->texture);
        }
        free(animation);
    }
}

PlayerAnimations *loadPlayerAnimations(SDL_Renderer *renderer)
{
    PlayerAnimations *animations = (PlayerAnimations *)malloc(sizeof(PlayerAnimations));
    if (animations == NULL)
    {
        return NULL;
    }

    animations->idle = NULL;
    animations->idle_left = NULL;
    animations->run = NULL;
    animations->run_left = NULL;
    animations->facingRight = 1;

    const char *paths[][4] = {
        {"asset/santa_sheets/idle_sheet.png",
         "asset/santa_sheets/run_sheet.png",
         "asset/santa_sheets/idle_left_sheet.png",
         "asset/santa_sheets/run_left_sheet.png"},
        {"asset/sprit/santa_sheets/idle_sheet.png",
         "asset/sprit/santa_sheets/run_sheet.png",
         "asset/sprit/santa_sheets/idle_left_sheet.png",
         "asset/sprit/santa_sheets/run_left_sheet.png"},
        {"asset/player/idle_sheet.png",
         "asset/player/run_sheet.png",
         "asset/player/idle_left_sheet.png",
         "asset/player/run_left_sheet.png"}};

    Animation *loadedIdle = NULL;
    Animation *loadedRun = NULL;

    for (int i = 0; i < 3 && (loadedIdle == NULL || loadedRun == NULL); i++)
    {
        if (loadedIdle == NULL)
        {
            loadedIdle = loadAnimation(renderer, paths[i][0], 6, 32, 32, 150);
        }

        if (loadedRun == NULL)
        {
            loadedRun = loadAnimation(renderer, paths[i][1], 6, 32, 32, 100);
        }
    }

    if (loadedIdle == NULL)
    {
        SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
        if (tempSurface != NULL)
        {
            SDL_FillRect(tempSurface, NULL, SDL_MapRGB(tempSurface->format, 255, 0, 0));

            loadedIdle = (Animation *)malloc(sizeof(Animation));
            if (loadedIdle != NULL)
            {
                loadedIdle->texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
                loadedIdle->frameCount = 1;
                loadedIdle->currentFrame = 0;
                loadedIdle->frameWidth = 32;
                loadedIdle->frameHeight = 32;
                loadedIdle->frameDuration = 150;
                loadedIdle->lastFrameTime = SDL_GetTicks();
            }
            SDL_FreeSurface(tempSurface);
        }
    }

    if (loadedRun == NULL && loadedIdle != NULL)
    {
        loadedRun = (Animation *)malloc(sizeof(Animation));
        if (loadedRun != NULL)
        {
            loadedRun->texture = loadedIdle->texture;
            loadedRun->frameCount = loadedIdle->frameCount;
            loadedRun->currentFrame = 0;
            loadedRun->frameWidth = loadedIdle->frameWidth;
            loadedRun->frameHeight = loadedIdle->frameHeight;
            loadedRun->frameDuration = 100;
            loadedRun->lastFrameTime = SDL_GetTicks();
        }
    }

    if (loadedIdle == NULL || loadedRun == NULL)
    {
        if (loadedIdle != NULL)
            freeAnimation(loadedIdle);
        if (loadedRun != NULL && loadedRun->texture != loadedIdle->texture)
            freeAnimation(loadedRun);
        free(animations);
        return NULL;
    }

    animations->idle = loadedIdle;
    animations->run = loadedRun;

    animations->idle_left = animations->idle;
    animations->run_left = animations->run;

    return animations;
}

void freePlayerAnimations(PlayerAnimations *animations)
{
    if (animations != NULL)
    {
        if (animations->idle != NULL)
        {
            if (animations->idle->texture != NULL &&
                (animations->idle_left == NULL || animations->idle->texture != animations->idle_left->texture) &&
                (animations->run == NULL || animations->idle->texture != animations->run->texture) &&
                (animations->run_left == NULL || animations->idle->texture != animations->run_left->texture))
            {
                SDL_DestroyTexture(animations->idle->texture);
            }
            free(animations->idle);
        }

        if (animations->idle_left != NULL && animations->idle_left != animations->idle)
        {
            if (animations->idle_left->texture != NULL &&
                (animations->run == NULL || animations->idle_left->texture != animations->run->texture) &&
                (animations->run_left == NULL || animations->idle_left->texture != animations->run_left->texture))
            {
                SDL_DestroyTexture(animations->idle_left->texture);
            }
            free(animations->idle_left);
        }

        if (animations->run != NULL && animations->run != animations->idle && animations->run != animations->idle_left)
        {
            if (animations->run->texture != NULL &&
                (animations->run_left == NULL || animations->run->texture != animations->run_left->texture))
            {
                SDL_DestroyTexture(animations->run->texture);
            }
            free(animations->run);
        }

        if (animations->run_left != NULL &&
            animations->run_left != animations->idle &&
            animations->run_left != animations->idle_left &&
            animations->run_left != animations->run)
        {
            if (animations->run_left->texture != NULL)
            {
                SDL_DestroyTexture(animations->run_left->texture);
            }
            free(animations->run_left);
        }

        free(animations);
    }
}

void loadCarnivoreAnimation(SDL_Renderer *renderer)
{
    if (carnivoreAnimation == NULL)
    {
        const char *carnivorePaths = "asset/Tiles/carnivore.png";

        carnivoreAnimation = loadAnimation(renderer, carnivorePaths, 2, 25, 25, 250);
        if (carnivoreAnimation == NULL)
        {
            SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
            if (tempSurface != NULL)
            {
                SDL_FillRect(tempSurface, NULL, SDL_MapRGB(tempSurface->format, 255, 0, 0));
                
                carnivoreAnimation = (Animation *)malloc(sizeof(Animation));
                if (carnivoreAnimation != NULL)
                {
                    carnivoreAnimation->texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
                    carnivoreAnimation->frameCount = 2;
                    carnivoreAnimation->currentFrame = 0;
                    carnivoreAnimation->frameWidth = 32;
                    carnivoreAnimation->frameHeight = 32;
                    carnivoreAnimation->frameDuration = 500;
                    carnivoreAnimation->lastFrameTime = SDL_GetTicks();
                }
                SDL_FreeSurface(tempSurface);
            }
        }
    }
}

void freeCarnivoreAnimation()
{
    if (carnivoreAnimation != NULL)
    {
        freeAnimation(carnivoreAnimation);
        carnivoreAnimation = NULL;
    }
}