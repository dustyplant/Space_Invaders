#include "TitleScreen.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <vector>
#include <string>

TitleScreen::TitleScreen(SDL_Surface* temp, int SCREEN_WIDTH_, int SCREEN_HEIGHT_){
	screen = temp;
	SCREEN_HEIGHT = SCREEN_HEIGHT_;
	SCREEN_WIDTH = SCREEN_WIDTH_;
}

void TitleScreen::cleanTitleScreen(SDL_Surface* temp1, TTF_Font* temp2){
	SDL_FreeSurface(temp1);
	TTF_CloseFont(temp2);
}

struct TitleScreen::MenuOptions{
	SDL_Rect box;
	SDL_Surface* image;
};

void TitleScreen::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination){
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, NULL, destination, &offset);
}

bool TitleScreen::titleScreen(bool &quit, bool &hardMode, bool &mediumMode, bool &easyMode, SDL_Event &event){

	SDL_Surface* titleName = NULL;
	TTF_Font *titleFont = NULL;
	TTF_Font *menuFont = NULL;
	//Purple-ish
	SDL_Color titleColor = {0xBF, 0x5F, 0xFF};

	titleFont = TTF_OpenFont("res/Fonts/arial.ttf", 50);
	if(titleFont == NULL){
		cleanTitleScreen(titleName, titleFont);
		return false;
	}

	titleName = TTF_RenderText_Solid(titleFont, "Space Invaders", titleColor);
	if(titleName == NULL){
		cleanTitleScreen(titleName, titleFont);
		return false;
	}
	
	menuFont = TTF_OpenFont("res/Fonts/arial.ttf", 20);
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
									if(displayControls(titleColor, quit, event) == false)
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

bool TitleScreen::displayControls(SDL_Color color, bool &quit, SDL_Event &event){
	//Clears the screen.
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0,0,0));

	SDL_Surface* ctrlMessage = NULL;
	TTF_Font* ctrlFont = TTF_OpenFont( "res/Fonts/arial.ttf", 50);
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
	ctrlFont = TTF_OpenFont( "res/Fonts/arial.ttf", 20);
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