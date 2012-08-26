/*
Written by: Shane Satterfield
08-22-2012

#TODO::Add different weapons.
#TODO::Add super-shield or last-resort shield.
#TOOO::Have a score board.
#TODO::Missle Mode.
#TODO::Sound effects/music and ability to mute it.
#TODO::Update the controls screen.
#TODO::The amount of shield that you have can change the weapon that you use.
#TODO::You can expend a certain amount of shield to fire a more powerful shot. It might be able to clear the screen.

*/
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include "src/AlienShots.h"
#include "src/Pilot.h"
#include "src/Shot.h"
#include "src/Alien.h"

#include <string>
#include <vector>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 608;
const int SCREEN_BPP = 32;

const int FRAMES_PER_SECOND = 60;
int frames_per = 0;

SDL_Surface* screen = NULL;
SDL_Surface* invader = NULL;
SDL_Surface* shots = NULL;
SDL_Surface* unit = NULL;
SDL_Surface* shieldedUnit = NULL;
SDL_Surface* explosion = NULL;
SDL_Surface* alienShots = NULL;
SDL_Surface* message = NULL;
SDL_Surface* score = NULL;
SDL_Surface* lowShield = NULL;

//Font for end game message.
TTF_Font *font = NULL;
//Font for the score.
TTF_Font *scoreFont = NULL;
//Bluish
SDL_Color scoreColor = {0, 0xEE, 0xEE};
SDL_Color shieldColor = {0, 0, 0};
//Maroon
SDL_Color textColor = {0x80, 0x00, 0x00};
SDL_Event event;

class Timer{
private:
	int startTicks;

public:
	Timer();
	void start();
	int get_ticks();
};

Timer::Timer(){
	startTicks = 0;
}

void Timer::start(){
	startTicks = SDL_GetTicks();
}

int Timer::get_ticks(){
	return SDL_GetTicks() - startTicks;
}

SDL_Surface* load_image(std::string filename){
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = IMG_Load(filename.c_str());
	if(loadedImage != NULL){
		optimizedImage = SDL_DisplayFormat(loadedImage);
		if(optimizedImage != NULL){
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format,0, 0, 0));
		}
		SDL_FreeSurface(loadedImage);
	}
	return optimizedImage;
}

bool load_files(){
	alienShots = load_image("res/Images/alienShot.bmp");
	if(alienShots == NULL)
		return false;
	invader = load_image("res/Images/black.jpg");
	if(invader == NULL)
		return false;
	
	shots = load_image("res/Images/shot.bmp");
	if(shots == NULL)
		return false;
	unit = load_image("res/Images/pilot.bmp");
	if(unit == NULL)
		return false;
	shieldedUnit = load_image("res/Images/shield.bmp");
	if(shieldedUnit == NULL)
		return false;
	lowShield = load_image("res/Images/lowShield.bmp");
	if(lowShield == NULL){
		return false;
	}
	explosion = load_image("res/Images/explosion.bmp");
	if(explosion == NULL)
		return false;
	font = TTF_OpenFont("res/fonts/armalite.ttf", 130);
	if(font == NULL)
		return false;
	scoreFont = TTF_OpenFont("res/fonts/arial.ttf", 20);
	if(scoreFont == NULL)
		return false;
	return true;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination){
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, NULL, destination, &offset);
}

void cleanup(){
	SDL_FreeSurface(shots);
	SDL_FreeSurface(unit);
	SDL_FreeSurface(shieldedUnit);
	SDL_FreeSurface(explosion);
	SDL_FreeSurface(invader);
	SDL_FreeSurface(alienShots);
	SDL_FreeSurface(message);
	SDL_FreeSurface(score);
	TTF_CloseFont(font);
	TTF_CloseFont(scoreFont);
	TTF_Quit();
	SDL_Quit();
}

bool init(){
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	if(screen == NULL)
		return false;
	if(TTF_Init() == -1)
		return false;

	return true;
}

void initAliens(std::vector<Alien> &alienVec){
	for(int b = 0, x = invader->w*1.5; b < 4;){
		Alien temp(x,b * invader->h * 1.5,invader);
		alienVec.push_back(temp);
		x += invader->w * 1.5;
		if(x >= SCREEN_WIDTH/2){
			b++;
			x = invader->w*1.5;
		}
	}
}

void moveAliens(std::vector<Alien> &alienVec, int mov, bool &endOfScreen, bool &dead){
	for(int i = 0; i < alienVec.size(); i++){
		if(alienVec[i].alive)
			apply_surface(alienVec[i].posx,alienVec[i].posy,alienVec[i].alien,screen);

		if(frames_per > 0){
            alienVec[i].posx += mov * ceil(float(FRAMES_PER_SECOND) / frames_per);
        }
        else
			alienVec[i].posx+=mov;

		if(alienVec[i].posx >= (SCREEN_WIDTH - alienVec[i].alien->w) - alienVec[i].alien->w * 1.5 || alienVec[i].posx < alienVec[i].alien->w*1.5){
			endOfScreen = true;
		}
		if(alienVec[i].posy + alienVec[i].alien->h >= SCREEN_HEIGHT)
			dead = true;
		if(alienVec[i].alive == false){
			apply_surface(alienVec[i].posx,alienVec[i].posy,explosion,screen);
			alienVec.erase(alienVec.begin() + i);
			--i;	
		}
	}
}

void checkHyperBeam(std::vector<Shot> &hyperBeam, std::vector<Alien> &alienVec, std::vector<AlienShots> &alienMissle){
	if(hyperBeam.size() > 0){
		for(int i = 0; i < hyperBeam.size(); ++i){
			if(hyperBeam[i].shot_hit == false){
				
				if(frames_per > 0){
			        hyperBeam[i].posy -= 5 * ceil(float(FRAMES_PER_SECOND) / frames_per);
			    }
			    else
					hyperBeam[i].posy -= 5;

				apply_surface(hyperBeam[i].posx, hyperBeam[i].posy, hyperBeam[i].image, screen);
				for(int j = 0; j < alienVec.size(); ++j){
					if(hyperBeam[i].collision(alienVec[j].posx, alienVec[j].posy, alienVec[j].alien->w, alienVec[j].alien->h)){
						hyperBeam[i].hit();
						
						alienVec[j].death();
						
					}
					
				}
			}
			if(hyperBeam[i].shot_hit || hyperBeam[i].posy < 0){
				hyperBeam[i].hit();
			}
			for(int j = 0; j < alienMissle.size(); ++j){
				if(hyperBeam[i].collision(alienMissle[j].getx(), alienMissle[j].gety(), alienMissle[j].image->w, alienMissle[j].image->h)){
					hyperBeam[i].hit();
				}
			}
			if(hyperBeam[i].shot_hit){
				hyperBeam.erase(hyperBeam.begin() + i);
				--i;
			}
		}
	}
}

struct MenuOptions{
	SDL_Rect box;
	SDL_Surface* image;
};

//For cleanup.
void cleanTitleScreen(SDL_Surface* temp1, TTF_Font* temp2){
	SDL_FreeSurface(temp1);
	TTF_CloseFont(temp2);
}

bool displayControls(SDL_Color color, bool &quit){
	//Clears the screen.
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0,0,0));

	SDL_Surface* ctrlMessage = NULL;
	TTF_Font* ctrlFont = TTF_OpenFont( "res/fonts/arial.ttf", 50);
	if(ctrlFont == NULL){		
		return false;
	}
	
	ctrlMessage = TTF_RenderText_Solid(ctrlFont, "Controls", color);
	if(ctrlMessage == NULL){
		cleanTitleScreen(ctrlMessage, ctrlFont);
		return false;
	}

	//Applies the title to the screen.
	apply_surface((SCREEN_WIDTH - ctrlMessage->w)/2, SCREEN_HEIGHT/5, ctrlMessage, screen);	

	SDL_FreeSurface(ctrlMessage);		

	TTF_CloseFont(ctrlFont);
	ctrlFont = TTF_OpenFont( "res/fonts/arial.ttf", 20);
	if(ctrlFont == NULL){
		return false;
	}

	//This holds the string values of all the menu options.
	std::vector<std::string> strNames;

	//Initializes strNames.
	strNames.push_back("Hold down the space bar to shoot.");
	strNames.push_back("Move with the left and right arrow keys.");
	strNames.push_back("You can also use the a and d keys to move.");
	strNames.push_back("Don't let the aliens reach the bottom of the screen, ");
	strNames.push_back("and don't let the green ships hit you.");
	strNames.push_back("See how long you can survive.");

	strNames.push_back("Go back.");

	std::vector<MenuOptions> vec;

	//Initializes vec and applies the images stored in the indexes of vec to the screen.
	for(int i = 0; i < strNames.size(); i++){

		MenuOptions men;
		vec.push_back(men);

		vec[i].image = TTF_RenderText_Solid(ctrlFont, strNames[i].c_str(), color);
		if(i > 0){
			vec[i].box.y = vec[i-1].box.y + vec[i-1].box.h; 
		}
		else{	
			vec[i].box.y = (SCREEN_HEIGHT - vec[i].image->h)/3;
		}
		vec[i].box.x = (SCREEN_WIDTH - vec[i].image->w)/2;
		vec[i].box.w = vec[i].image->w;
		vec[i].box.h = vec[i].image->h;

		apply_surface( vec[i].box.x, vec[i].box.y , vec[i].image, screen );
	}
	

	if(SDL_Flip(screen) == -1){
		return false;
	}

	bool start = false;
	int x = 0;
	int y = 0;
	while(!quit && !start){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}
			//Press enter to go back to the main menu.
			if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym ){
					case SDLK_RETURN:
						start = true;
						break;
				}
			}

			//For when the user clicks one of the menu options.
			//This is setup for expansion.
			//It is currently only used when the player presses to go back to the main menu.
			if(event.type == SDL_MOUSEBUTTONUP){
				if(event.button.button == SDL_BUTTON_LEFT){
					x = event.button.x;
					y = event.button.y;
					for(int i = 0; i < vec.size(); ++i){
						if(x > vec[i].box.x && x < vec[i].box.x + vec[i].box.w && y > vec[i].box.y && y < vec[i].box.y + vec[i].box.h){
							if(i + 1 == vec.size())
								start = true;
						}
					}
				}
			}
		}
	}
	return true;
}

bool titleScreen(bool &quit, bool &hardMode, bool &mediumMode, bool &easyMode){


	SDL_Surface* titleName = NULL;
	TTF_Font *titleFont = NULL;
	TTF_Font *menuFont = NULL;
	//Purple-ish
	SDL_Color titleColor = {0xBF, 0x5F, 0xFF};

	titleFont = TTF_OpenFont("res/fonts/arial.ttf", 50);
	if(titleFont == NULL){
		cleanTitleScreen(titleName, titleFont);
		return false;
	}

	titleName = TTF_RenderText_Solid(titleFont, "Space Invaders", titleColor);
	if(titleName == NULL){
		cleanTitleScreen(titleName, titleFont);
		return false;
	}
	
	menuFont = TTF_OpenFont("res/fonts/arial.ttf", 20);
	if(menuFont == NULL){
		cleanTitleScreen(titleName, menuFont);
		return false;
	}

	bool start = false;
	int x = 0;
	int y = 0;
	std::vector<std::string> strNames;
	std::vector<MenuOptions> vec;

	//Initializes strNames to hold the string values of the menu options.
	strNames.push_back("Press Enter to Start Game");
	strNames.push_back("Hard Mode");
	strNames.push_back("Medium Mode");
	strNames.push_back("Easy Mode");
	strNames.push_back("Controls");
	strNames.push_back("Quit");

	//Initializes vec to hold the menu items.
	for(int i = 0; i < strNames.size(); i++){

		MenuOptions men;
		vec.push_back(men);

		vec[i].image = TTF_RenderText_Solid(menuFont, strNames[i].c_str(), titleColor);
		if(i > 0){
			vec[i].box.y = vec[i-1].box.y + vec[i-1].box.h + SCREEN_HEIGHT/30; 
		}
		else{	
			vec[i].box.y = (SCREEN_HEIGHT - vec[i].image->h)/2.5;
		}
		vec[i].box.x = (SCREEN_WIDTH - vec[i].image->w)/2;
		vec[i].box.w = vec[i].image->w;
		vec[i].box.h = vec[i].image->h;

	}
		
	while(!quit && !start){
		//Clears the screen.
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0,0,0));

		//Write the title to the screen.
		apply_surface((SCREEN_WIDTH - titleName->w)/2, SCREEN_HEIGHT/5, titleName, screen);

		//Writes the menu items.
		for(int i = 0; i < vec.size(); i++)
			apply_surface( vec[i].box.x, vec[i].box.y , vec[i].image, screen );

		if(SDL_Flip(screen) == -1){
			return false;
		}

		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}

			//Starts the game when the player presses enter.
			if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym ){
					case SDLK_RETURN:
						start = true;
						break;
				}
			}

			//This is for clicking the options.
			if(event.type == SDL_MOUSEBUTTONUP){
				if(event.button.button == SDL_BUTTON_LEFT){
					x = event.button.x;
					y = event.button.y;
					for(int i = 0; i < vec.size(); ++i){
						if(x > vec[i].box.x && x < vec[i].box.x + vec[i].box.w && y > vec[i].box.y && y < vec[i].box.y + vec[i].box.h){
							//std::cout << strNames[i] << std::endl;
							switch(i){
								case 0:
									start = true;
									break;
								case 1:
									hardMode = true;
									mediumMode = false;
									easyMode = false;
									start = true;
									break;
								case 2:
									hardMode = false;
									mediumMode = true;
									easyMode = false;
									start = true;
									break;
								case 3:
									hardMode = false;
									mediumMode = false;
									easyMode = true;
									start = true;
									break;
								case 4:
									if(displayControls(titleColor, quit) == false)
										return false;
									break;
								case 5:
									quit = true;
							}
						}
					}
				}
			}
		}
	}

	
	//Cleanup
	for(int i = 0; i < vec.size(); ++i){
		SDL_FreeSurface(vec[i].image);
	}

	TTF_CloseFont(menuFont);
	TTF_CloseFont(titleFont);
	SDL_FreeSurface(titleName);

	return true;
}

int main(int argc, char* args[]){

	int frames = 0;

	srand(time(NULL));
	if(init() == false)
		return 1;

	if(load_files() == false)
		return 1;

	bool quit = false;
	bool dead = false;

	bool hardMode = true;
	bool mediumMode = false;
	bool easyMode = false;

	frames_per = 0;
	do{
		//True if the player closed the window.
		quit = false;
		//True if the player lost.
		bool dead = false;

		if(titleScreen(quit, hardMode, mediumMode, easyMode) == false){
			return 1;
		}

		//Initialize Pilot
		Pilot pilot((SCREEN_WIDTH - unit->w)/2, SCREEN_HEIGHT - unit->h, unit, shieldedUnit, lowShield, 0);

		//Initialize Alien Vector
		std::vector<Alien> alienVec;
		initAliens(alienVec);

		std::vector<Shot> hyperBeam;

		int varSpeed = 5;
		int speedUp = 5;
		int speedTimerAdder = 5;
		int mov = invader->w*.1;
		int slice = 3;
		int shieldRate = 0;
		int points = 0;
		double ptMult = 1;
		pilot.shield = 1;


		if(mediumMode){
			varSpeed = 3;
			speedUp = 10;
			speedTimerAdder = 8;
			mov = invader->w * .1;
			slice = 5;
			pilot.shield = 0;
			shieldRate = 45;
			ptMult = .5;
		}

		if(easyMode){
			varSpeed = 2;
			speedUp = 10;
			speedTimerAdder = 10;
			mov = invader->w * .075;
			slice = 4;
			pilot.shield = 3;
			shieldRate = 15;
			ptMult = .2;
		}

		pilot.hardMode = hardMode;

		int shieldRate2 = shieldRate;

		//Initializing the two alien missles.
		std::vector<AlienShots> alienMissle;
		AlienShots temp( rand()%SCREEN_WIDTH - alienShots->w, alienShots->h, varSpeed, alienShots);
		alienMissle.push_back(temp);
		AlienShots temp3( rand()%SCREEN_WIDTH - alienShots->w, alienShots->h, varSpeed, alienShots);
		alienMissle.push_back(temp3);

		//For movement of the Aliens
		bool endOfScreen = false;

		//Framerate regulation
		frames = 0;
		frames_per = 0;
		bool cap = true;
		Timer fps;
		Timer update;
		Timer keeper;
		clock_t survivalTime = clock();
		clock_t t = clock();
		clock_t shotPause = clock();
		update.start();
		keeper.start();

		while(!quit && !dead){
			//To increase the speed of the alien missles.
			if( int(((float(clock()) - float(survivalTime))/CLOCKS_PER_SEC)) % speedUp == 0 && int(((float(clock()) - float(survivalTime))/CLOCKS_PER_SEC)) != 0){
				if(frames_per > 0){
		            varSpeed += 1 * ceil(float(FRAMES_PER_SECOND) / frames_per);
		        }
		        else
					varSpeed++;
				speedUp += speedTimerAdder;
				for(int i = 0; i < alienMissle.size(); ++i){
					alienMissle[i].set_speed(varSpeed);
				}
			}

			/*
			if( !hardMode && int(((float(clock()) - float(survivalTime))/CLOCKS_PER_SEC)) % shieldRate == 0 && int(((float(clock()) - float(survivalTime))/CLOCKS_PER_SEC)) != 0){
				pilot.shield++;
				shieldRate += shieldRate2;
			}*/

			if(alienVec.size() <= 0){
				initAliens(alienVec);
				pilot.shield++;
			}
			fps.start();

			//Background
			SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0,0,0));
			//Displaying Aliens
			moveAliens(alienVec, mov, endOfScreen, dead);

			//When the Aliens reach the site of a screen.
			if(endOfScreen){
				mov*=-1;
				for(int j = 0; j < alienVec.size(); j++){
					alienVec[j].posx+=mov;
					alienVec[j].posy+= alienVec[j].alien->h * 1.5;
					
				}
				endOfScreen = false;
			}

			std::stringstream streamTempMessage;
			streamTempMessage << pilot.shield;

			SDL_Surface* tempMessage = TTF_RenderText_Solid(scoreFont, streamTempMessage.str().c_str(), shieldColor);
			//Displays the pilot.
			pilot.show(screen, tempMessage);

			//For the alien's missle.
			for(int i = 0; i < alienMissle.size(); ++i){
				alienMissle[i].show(screen);

				alienMissle[i].move(pilot.get_rects(), SCREEN_HEIGHT);
				if(alienMissle[i].getHit()){
					alienMissle[i].unHit();
					alienMissle[i].reset(rand()%int(SCREEN_WIDTH - alienShots->w - unit->w) + int(unit->w/2), rand()%(SCREEN_HEIGHT - alienShots->h) / slice);
				}

				if(pilot.check_collision(alienMissle[i].get_rects())){
					if(hardMode || pilot.shield <= 0)
						dead = true;
					else
						pilot.shield--;
				}
			}

			//Check to see if the shot hit anything.
			checkHyperBeam(hyperBeam, alienVec, alienMissle);



			std::stringstream streamer;
			streamer << "Speed: " << varSpeed << "     Survival Time: " << ((float(clock()) - float(survivalTime))/CLOCKS_PER_SEC);
			std::stringstream streamer2;
			streamer2 << "FPS: " << frames_per << "     Shield: " << pilot.shield << "     Score: " << points;
			score = TTF_RenderText_Solid(scoreFont, streamer.str().c_str(), scoreColor);
			
			apply_surface(0,0,score, screen);

			message = TTF_RenderText_Solid(scoreFont, streamer2.str().c_str(), scoreColor);

			apply_surface(0,score->h, message, screen);

			pilot.pilotMovement(hyperBeam, shots, shotPause, SCREEN_WIDTH, FRAMES_PER_SECOND, frames_per);


			if(SDL_Flip(screen) == -1)
				return 1;			

			while(SDL_PollEvent(&event)){
				//To close the window.
				if(event.type == SDL_QUIT){
					quit = true;
				}
			}
			if(update.get_ticks() > 1000){
				frames_per = frames / (keeper.get_ticks() / 1000.f);
				points += (pilot.shield + varSpeed) * ptMult;
				

				update.start();
			}

			frames++;

			//Framerate regulation.
			if(cap == true && fps.get_ticks() < 1000.f / FRAMES_PER_SECOND){
				SDL_Delay((1000.0/FRAMES_PER_SECOND) - fps.get_ticks());
			}
		}

		if(dead && !quit){
			//Lose message.
			message = TTF_RenderText_Solid(font, "Game Over", textColor);
			apply_surface((SCREEN_WIDTH - message->w)/2,(SCREEN_HEIGHT - message->h)/2,message, screen);
			if(SDL_Flip(screen) == -1)
				return 1;
			while(!quit && dead){
				while(SDL_PollEvent(&event)){
					//To close the window.
					if(event.type == SDL_QUIT){
						quit = true;
					}
					else if(event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONUP){
						dead = false;
					}
					/*
					if(event.type == SDL_KEYDOWN){
						switch(event.key.keysym.sym ){
							case SDLK_RETURN:
								dead = false;
								break;
						}
					}*/
				}
			}
		}
	}while(!quit);

	cleanup();
	return 0;

}