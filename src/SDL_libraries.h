#pragma once

#if _MSC_VER 
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
	#include <SDL_mixer.h>
	#include <SDL_pixels.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_pixels.h>
#endif