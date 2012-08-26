#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <vector>
#include <string>

class TitleScreen{
private:
	SDL_Surface* screen;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	struct MenuOptions;
	void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);

public:
	TitleScreen(SDL_Surface* temp, int SCREEN_WIDTH_, int SCREEN_HEIGHT_);
	bool displayControls(SDL_Color color, bool &quit, SDL_Event &event);
	bool titleScreen(bool &quit, bool &hardMode, bool &mediumMode, bool &easyMode, SDL_Event &event);
	void cleanTitleScreen(SDL_Surface* temp1, TTF_Font* temp2);
};

#endif