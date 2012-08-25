#ifndef ALIEN_H
#define ALIEN_H

class Alien{
public:
	int posx;
	int posy;
	bool alive;
	SDL_Surface* alien;
	Alien(int x, int y, SDL_Surface* image);
	void death();
};

#endif