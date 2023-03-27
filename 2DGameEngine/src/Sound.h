#pragma once
#include <SDL_mixer.h>
#include <iostream>

class Sound
{
public:	
	virtual void Play(int loops) = 0;
	virtual void Stop() = 0;
};

class Music : public Sound
{
private:
	Mix_Music* music = nullptr;

public:
	Music(const char* filename)
	{
		music = Mix_LoadMUS(filename);		

		//Mix_Volume(0, 128);
		if (!music)
		{
			std::cout << "Music Error: " << Mix_GetError() << std::endl;
		}
	}

	void Play(int loops = -1) override
	{
		Mix_PlayMusic(music, loops); // -1 = loops indefinitely
	}

	void Stop() override
	{
		Mix_HaltMusic();
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

	void Play(int loops = 0) override
	{
		Mix_PlayChannel(-1, sound, loops); // -1 = first available channel
	}

	void Stop() override
	{
		Mix_HaltChannel(-1);
	}
};