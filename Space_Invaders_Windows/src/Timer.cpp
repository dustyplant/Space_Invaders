#include "Timer.h"


Timer::Timer(){
	startTicks = 0;
}

void Timer::start(){
	startTicks = SDL_GetTicks();
}

int Timer::get_ticks(){
	return SDL_GetTicks() - startTicks;
}