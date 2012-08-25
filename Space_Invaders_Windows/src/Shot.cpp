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

Shot::Shot(int x, int y, SDL_Surface* temp){
	posx = x;
	posy = y;
	image = temp;
	shot_hit = false;
}
bool Shot::collision(int x, int y, int w, int h){
	if( ( (posx > x && posx < x + w) || (posx + image->w > x && posx + image->w < x + w) ) && ( (posy > y && posy < y + h) || (posy + image->h > y && posy + image->h < y + h))){
		return true;
	}

	return false;
}
void Shot::hit(){
	shot_hit = true;
}