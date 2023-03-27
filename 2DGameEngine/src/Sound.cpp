#include "sound.h"

int musicVolume = 64;
int soundVolume = 128;

int lastMusicVolume = 0;

bool muted = false;

void Sound::VolumeUp(int amount)
{
	musicVolume += amount;

	if (musicVolume > 128)
		musicVolume = 128;

	Mix_VolumeMusic(musicVolume);
	std::cout << "Music Volume: " << musicVolume << std::endl;

	soundVolume += amount;

	if (soundVolume > 256)
		soundVolume = 256;

	Mix_Volume(-1, soundVolume);
	std::cout << "Sound Effect Volume: " << soundVolume << std::endl;
}

void Sound::VolumeDown(int amount)
{
	musicVolume -= amount;
	if (musicVolume < -128)
		musicVolume = -128;

	Mix_VolumeMusic(musicVolume);
	std::cout << "Music Volume: " << musicVolume << std::endl;

	soundVolume -= amount;
	if (soundVolume < 0)
		soundVolume = 0;

	Mix_Volume(-1, soundVolume);
	std::cout << "Sound Effect Volume: " << soundVolume << std::endl;
}

void SoundEffect::Play(int loops)
{
	Mix_Volume(-1, soundVolume);
	Mix_PlayChannel(-1, sound, loops); // -1 = first available channel
}

void Music::Play(int loops)
{
	Mix_VolumeMusic(musicVolume);
	Mix_PlayMusic(music, loops); // -1 = loops indefinitely
}

void Music::VolumeUp(int amount)
{
	musicVolume += amount;

	if (musicVolume > 128)
		musicVolume = 128;

	Mix_VolumeMusic(musicVolume);
	std::cout << "Music Volume: " << musicVolume << std::endl;
}

void Music::VolumeDown(int amount)
{
	musicVolume -= amount;
	if (musicVolume < 0)
		musicVolume = 0;

	Mix_VolumeMusic(musicVolume);
	std::cout << "Music Volume: " << musicVolume << std::endl;
}

void Music::ToggleMute()
{
	if (!muted)
	{
		lastMusicVolume = musicVolume;
		musicVolume = 0;		
	}
	else
	{
		musicVolume = lastMusicVolume;		
	}

	Mix_VolumeMusic(musicVolume);
	muted = !muted;
}

void SoundEffect::VolumeUp(int amount)
{
	if (soundVolume > 128)
		soundVolume = 128;

	Mix_Volume(-1, soundVolume);
	std::cout << "Sound Effect Volume: " << musicVolume << std::endl;
}

void SoundEffect::VolumeDown(int amount)
{
	soundVolume -= amount;
	if (soundVolume < 0)
		soundVolume = 0;

	Mix_Volume(-1, soundVolume);
	std::cout << "Sound Effect Volume: " << musicVolume << std::endl;
}