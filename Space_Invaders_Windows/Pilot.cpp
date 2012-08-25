#include "Pilot.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "Shot.h"
#include <vector>
#include <time.h>

Pilot::Pilot( int X, int Y, SDL_Surface* temp)
{
    //Initialize the offsets
    x = X;
    y = Y;
    
    //Initialize the velocity
    xVel = 0;
    yVel = 0;

    image = temp;
    
    //Create the necessary SDL_Rects
    box.resize( 6 );
    
    //Initialize the collision boxes' width and height
    box[ 0 ].w = 4;
    box[ 0 ].h = 12;
    
    box[ 1 ].w = 3*4;
    box[ 1 ].h = 4;
    
    box[ 2 ].w = 20;
    box[ 2 ].h = 4;
    
    box[ 3 ].w = 36;
    box[ 3 ].h = 4;
    
    box[ 4 ].w = 52;
    box[ 4 ].h = 4;
    
    box[ 5 ].w = 68;
    box[ 5 ].h = 20;
    
    //Move the collision boxes to their proper spot
    shift_boxes();
}

void Pilot::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination){
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(source, NULL, destination, &offset);
}

void Pilot::shift_boxes()
{
    //The row offset
    int r = 0;
    
    //Go through the dot's collision boxes
    for( int set = 0; set < box.size(); set++ )
    {
        //Center the collision box
        box[ set ].x = x + ( image->w - box[ set ].w ) / 2;
        
        //Set the collision box at its row offset
        box[ set ].y = y + r;
        
        //Move the row offset down the height of the collision box
        r += box[ set ].h;    
    }
}

void Pilot::pilotMovement(std::vector<Shot> &hyperBeam, SDL_Surface* shots, clock_t &shotPause, int SCREEN_WIDTH, int FRAMES_PER_SECOND, int frames_per){
    Uint8 *keystates = SDL_GetKeyState(NULL);

    if( (keystates[SDLK_RIGHT] || keystates[SDLK_d] ) && x + image->w < SCREEN_WIDTH){
        if(frames_per > 0 && frames_per < FRAMES_PER_SECOND){
            x += 10 * (float(FRAMES_PER_SECOND) / frames_per);
        }
        else
            x += 10;
        shift_boxes();
    }
    if( (keystates[SDLK_LEFT] || keystates[SDLK_a]) && x > 0){
        if(frames_per > 0 && frames_per < FRAMES_PER_SECOND){
            x -= 10 * (float(FRAMES_PER_SECOND) / frames_per);
        }
        else
            x -= 10;
        shift_boxes();
    }
    if(keystates[SDLK_SPACE] && (float(clock()) - float(shotPause))/CLOCKS_PER_SEC > .3){
        Shot temp2(x + (image->w - shots->w)/2, y, shots);
        hyperBeam.push_back(temp2);
        shotPause = clock();
    }   
}

void Pilot::show(SDL_Surface* screen){
    apply_surface(x,y,image,screen);
}

std::vector<SDL_Rect> &Pilot::get_rects()
{
    //Retrieve the collision boxes
    return box;    
}

bool Pilot::check_collision( std::vector<SDL_Rect> &B )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Go through the A boxes
    for( int Abox = 0; Abox < box.size(); Abox++ )
    {
        //Calculate the sides of rect A
        leftA = box[ Abox ].x;
        rightA = box[ Abox ].x + box[ Abox ].w;
        topA = box[ Abox ].y;
        bottomA = box[ Abox ].y + box[ Abox ].h;
        
        //Go through the B boxes    
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
    
    //If neither set of collision boxes touched
    return false;
}