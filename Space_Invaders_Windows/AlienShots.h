#ifndef ALIENSHOTS_H
#define ALIENSHOTS_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <vector>

class AlienShots{
private:
	int x;
	int y;
	bool hit;
	std::vector<SDL_Rect> box;

	int xVel, yVel;
	void shift_boxes();
public:
	AlienShots(int X, int Y, int varSpeed, SDL_Surface* temp);

	void move(std::vector<SDL_Rect> &rects, int SCREEN_HEIGHT);

	void set_speed(int speed);

	void show(SDL_Surface* screen);

	bool check_collision(std::vector<SDL_Rect> &B);

	void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);

	std::vector<SDL_Rect> &get_rects();

	bool getHit();

	void unHit();

	void reset(int X, int Y);

	int getx();

	int gety();
	
	SDL_Surface* image;
};

#endif