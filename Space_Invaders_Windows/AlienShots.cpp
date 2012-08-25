#include "AlienShots.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <vector>

AlienShots::AlienShots(int X, int Y, int varSpeed, SDL_Surface* temp){
	x = X;
	y = Y;

	xVel = 0;
	yVel = varSpeed;

	hit = false;

	image = temp;

	box.resize(8);

	box[ 0 ].w = 15*4;
	box[ 0 ].h = 20;

	box[ 1 ].w = 13*4;
	box[ 1 ].h = 4;

	box[ 2 ].w = 11*4;
	box[ 2 ].h = 4;

	box[ 3 ].w = 9*4;
	box[ 3 ].h = 4;

	box[ 4 ].w = 7*4;
	box[ 4 ].h = 4;

	box[ 5 ].w = 5*4;
	box[ 5 ].h = 4;

	box[ 6 ].w = 3*4;
	box[ 6 ].h = 4;

	box[ 7 ].w = 1*4;
	box[ 7 ].h = 4;

	shift_boxes();
}

int AlienShots::getx(){
	return x;
}

int AlienShots::gety(){
	return y;
}

void AlienShots::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination){
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(source, NULL, destination, &offset);
}

void AlienShots::set_speed(int speed){
	yVel = speed;
}

void AlienShots::shift_boxes(){
	//the row offset
	int r = 0;

	for(int set = 0; set < box.size(); set++){
		box[set].x = x + (60 - box[set].w)/2;

		box[set].y = y + r;

		r+= box[set].h;
	}
}

bool AlienShots::getHit(){
	return hit;
}

void AlienShots::unHit(){
	hit = false;
}

void AlienShots::reset(int X, int Y){
	x = X;
	y = Y;
}

void AlienShots::move(std::vector<SDL_Rect> &rects, int SCREEN_HEIGHT){
	y += yVel;

	shift_boxes();

	if(y < 0 || y + image->h> SCREEN_HEIGHT || check_collision(rects)){
		hit = true;
	}
}

void AlienShots::show(SDL_Surface* screen){
	apply_surface(x, y, image, screen);
}

std::vector<SDL_Rect> &AlienShots::get_rects(){
	return box;
}

bool AlienShots::check_collision(std::vector<SDL_Rect> &B){
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	for(int Abox = 0; Abox < box.size(); Abox++){
		leftA = box[ Abox ].x;
        rightA = box[ Abox ].x + box[ Abox ].w;
        topA = box[ Abox ].y;
        bottomA = box[ Abox ].y + box[ Abox ].h;
        for( int Bbox = 0; Bbox < B.size(); Bbox++ )
        {
            //Calculate the sides of rect B
            leftB = B[ Bbox ].x;
            rightB = B[ Bbox ].x + B[ Bbox ].w;
            topB = B[ Bbox ].y;
            bottomB = B[ Bbox ].y + B[ Bbox ].h;
            
            //If no sides from A are outside of B
            if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
            {
                //A collision is detected
                return true;
            }
        }
	}
	return false;
}