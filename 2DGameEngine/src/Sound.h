#pragma once
#include <SDL_mixer.h>
#include <iostream>

class Sound
{
public:	
	virtual void Play() = 0;
};

class Music : public Sound
{
private:
	Mix_Music* music = nullptr;

public:
	Music(const char* filename)
	{
		music = Mix_LoadMUS(filename);
		if (!music)
		{
			std::cout << "Music Error: " << Mix_GetError() << std::endl;
		}
	}

	void Play() override
	{
		Mix_PlayMusic(music, -1); // -1 = loops indefinitely
	}
};

class SoundEffect : public Sound
{
private:
	Mix_Chunk* sound = nullptr;

public:
	SoundEffect(const char* filename)
	{
		sound = Mix_LoadWAV(filename);
		if (!sound)
		{
			std::cout << "Sound Error: " << Mix_GetError() << std::endl;
		}
	}

	void Play() override
	{
		Mix_PlayChannel(-1, sound, 0); // -1 = first available channel
	}
};