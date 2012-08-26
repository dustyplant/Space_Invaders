#! /bin/bash
g++ -c Space_Invaders.cpp
mv Space_Invaders.o src/obj
cd src
for files in *.cpp
	do
		g++ -c $files obj
done
mv *.o obj
cd obj
g++ -o Space_Invaders Space_Invaders.o Alien.o AlienShots.o Shot.o Pilot.o TitleScreen.o Timer.o -lSDL -lSDL_image -lSDL_ttf
mv Space_Invaders ..
cd ..
mv Space_Invaders ..
cd ..