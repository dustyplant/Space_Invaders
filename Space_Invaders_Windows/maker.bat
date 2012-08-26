g++ -c Space_Invaders.cpp
move Space_Invaders.o src/object_files
cd src
g++ -c AlienShots.cpp
g++ -c Alien.cpp
g++ -c Shot.cpp
g++ -c Pilot.cpp

move *.o object_files
cd object_files
g++ -o Space_Invaders.exe Space_Invaders.o AlienShots.o Alien.o Shot.o Pilot.o -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf
del *.o
move Space_Invaders.exe ..
cd ..
move Space_Invaders.exe ..
cd ..