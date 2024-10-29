#pragma once

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();
    void init();
    void loadSound(const char* path, int index);
    void loadMusic(const char* path, int index);
    void playSound(int index);
    void playMusic(int index, bool repeat);
    void unloadSound(int index);
    void unloadMusic(int index);
    void unloadAllSound();
    void unloadAllMusic();
    void closeAudioDevice();
private:
    std::vector<Mix_Chunk*> loadedSounds = {};
    std::vector<Mix_Music*> loadedMusic = {};
};

extern SoundManager *SM;
