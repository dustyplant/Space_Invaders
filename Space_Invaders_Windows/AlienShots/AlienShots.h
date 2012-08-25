#ifndef ALIENSHOTS_H
#define ALIENSHOTS_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <vector>

class AlienShots{
private:
	int posx;
	int posy;
	bool hit;
	SDL_Surface* image;
	std::vector<SDL_Rect> box;

	int xVel, yVel;
	void shift_boxes();
public:
	AlienShots(int x, int y, int varSpeed, SDL_Surface* temp);

	void move(std::vector<SDL_Rect> & rects);

	void set_speed(int speed);

	void show(SDL_Surface* screen);

	bool check_collision(std::vector<SDL_Rect> &B);

	std::vector<SDL_Rect> &get_rects();

	bool getHit();

	void unHit();

	void reset(int X, int Y);
};

#endif