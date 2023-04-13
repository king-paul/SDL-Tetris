#pragma once
#include "SDL_libraries.h"
#include <iostream>

class Sound
{
public:	
	virtual void Play(int loops = 0) = 0;
	virtual void Stop() = 0;

	static void VolumeUp(int amount);
	static void VolumeDown(int amount);
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

	~Music()
	{
		Mix_FreeMusic(music);
	}

	void Play(int loops = -1) override;

	void Stop() override
	{
		Mix_HaltMusic();
	}
	
	static void VolumeUp(int amount);
	static void VolumeDown(int amount);

	static void ToggleMute();
	
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

	~SoundEffect()
	{
		Mix_FreeChunk(sound);
	}

	void Play(int loops = 0) override;

	void Stop() override
	{
		Mix_HaltChannel(-1);
	}
	
	static void VolumeUp(int amount);
	static void VolumeDown(int amount);
};