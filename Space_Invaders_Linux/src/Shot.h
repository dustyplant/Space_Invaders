#ifndef SHOT_H
#define SHOT_H

class Shot{
public:
	int posx;
	int posy;
	bool shot_hit;
	SDL_Surface* image;
	Shot(int x, int y, SDL_Surface* temp);
	bool collision(int x, int y, int w, int h);
	void hit();
};

#endif