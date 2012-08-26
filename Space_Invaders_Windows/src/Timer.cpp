#include "Timer.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

Timer::Timer(){
	startTicks = 0;
}

void Timer::start(){
	startTicks = SDL_GetTicks();
}

int Timer::get_ticks(){
	return SDL_GetTicks() - startTicks;
}