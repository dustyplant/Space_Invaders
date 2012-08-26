#ifndef PILOT_H
#define PILOT_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "Shot.h"
#include <vector>
#include <time.h>

class Pilot{
private:
    //The offsets of the dot
    int x, y;

    SDL_Surface* image;

    SDL_Surface* shieldImage;

    SDL_Surface* lowShield;
    
    //The collision boxes of the dot
    std::vector<SDL_Rect> box;
    
    //The velocity of the dot
    int xVel, yVel;
    
    //Moves the collision boxes relative to the dot's offset
    void shift_boxes();
    
public:
    //Initializes the variables
    Pilot( int X, int Y, SDL_Surface* temp, SDL_Surface* shieldTemp, SDL_Surface* lowTemp, int shield_);
    
    //Takes key presses and adjusts the dot's velocity
    void pilotMovement(std::vector<Shot> &hyperBeam, SDL_Surface* shots, clock_t &shotPause, int SCREEN_WIDTH, int FRAMES_PER_SECOND, int frames_per);
    
    //Shows the dot on the screen
    void show(SDL_Surface* screen, SDL_Surface* tempMessage);

    void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);

    bool check_collision( std::vector<SDL_Rect> &A);

    int shield;
    
    //Gets the collision boxes
    std::vector<SDL_Rect> &get_rects();
};

#endif