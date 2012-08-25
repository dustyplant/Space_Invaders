#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include "Timer.h"
#include "Shot.h"
#include "Alien.h"

#include <string>
#include <vector>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

Alien::Alien(int x, int y, SDL_Surface* image){
	posx = x;
	posy = y;
	alien = image;
	alive = true;
}

void Alien::death(){
	alive = false;
}