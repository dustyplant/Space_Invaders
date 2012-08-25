#ifndef PILOT_H
#define PILOT_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <vector>
#include <time.h>

class Pilot{
private:
    //The offsets of the dot
    int x, y;
    
    //The collision boxes of the dot
    std::vector<SDL_Rect> box;
    
    //The velocity of the dot
    int xVel, yVel;
    
    //Moves the collision boxes relative to the dot's offset
    void shift_boxes();
    
public:
    //Initializes the variables
    Dot( int X, int Y );
    
    //Takes key presses and adjusts the dot's velocity
    void Pilot::pilotMovement(std::vector<Shot> &hyperBeam, SDL_Surface* shots, clock_t &shotPause, int SCREEN_WIDTH);
    
    //Shows the dot on the screen
    void show(SDL_Surface* screen);

    bool check_collision( std::vector<SDL_Rect> &A);
    
    //Gets the collision boxes
    std::vector<SDL_Rect> &get_rects();
};

#endif