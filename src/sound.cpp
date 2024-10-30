#include "game.hpp"

SoundManager::SoundManager() {init();}

SoundManager::~SoundManager()
{
    SM->unloadAllSound();
    SM->unloadAllMusic();
    SM->closeAudioDevice();
}

void SoundManager::init()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {std::cout << "SDL_mixer initialization blew its speakers up: " << Mix_GetError() << std::endl;}
    if (!(Mix_Init(MIX_INIT_FLAC) & MIX_INIT_FLAC)) {std::cout << "SDL_mixer FLAC initialization failed (no support?? noob): " << Mix_GetError() << std::endl;}
}

void SoundManager::loadSound(const char* path, int index)
{
    if (index >= loadedSounds.size()) {
        loadedSounds.resize(index + 1);
    }

    if (loadedSounds.at(index) != nullptr) {
        unloadSound(index);
    }

    Mix_Chunk* sound = Mix_LoadWAV(path);

    if (!sound) {std::cout << "sound file said nuh uh: " << Mix_GetError() << std::endl;return;}

    loadedSounds[index] = sound;
}

void SoundManager::loadMusic(const char* path, int index)
{
    if (index >= loadedMusic.size()) {
        loadedMusic.resize(index + 1);
    }
    if (loadedMusic.at(index) != nullptr) {
        unloadMusic(index);

    }

    Mix_Music* music = Mix_LoadMUS(path);

    if (!music) {std::cout << "flac file exploded: " << Mix_GetError() << std::endl;return;}

    loadedMusic[index] = music;
}

void SoundManager::playSound(int index)
{
   if (index < loadedSounds.size() && loadedSounds[index] != nullptr) {
        if (Mix_PlayChannel(-1, loadedSounds[index], 0) == -1) {
            std::cout << "bro ur cooked no sound!!: " << Mix_GetError() << std::endl;
        }
    }
}

void SoundManager::playMusic(int index, bool repeat)
{
    if (index < loadedMusic.size() && loadedMusic[index] != nullptr) {
        if (Mix_PlayMusic(loadedMusic[index], repeat ? -1 : 0) == -1) {
            std::cout << "Error playing music: " << Mix_GetError() << std::endl;
        }
    }
}

void SoundManager::unloadSound(int index)
{
    if (index < loadedSounds.size()) {
        Mix_FreeChunk(loadedSounds[index]);
        loadedSounds.erase(loadedSounds.begin() + index);
    }
}

void SoundManager::unloadMusic(int index)
{
    if (index < loadedMusic.size())
    {
        Mix_FreeMusic(loadedMusic[index]);
        loadedMusic.erase(loadedMusic.begin() + index);
    }
}

void SoundManager::unloadAllSound()
{
    for(int i = 0; i < loadedSounds.size(); i++) {
        unloadSound(i);
    }
    loadedSounds.clear();
}


void SoundManager::unloadAllMusic()
{
    for(int i = 0; i < loadedMusic.size(); i++) {
        unloadMusic(i);
    }
    loadedMusic.clear();
}

void SoundManager::closeAudioDevice()
{
    Mix_CloseAudio();
}
