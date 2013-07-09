GCC			= g++ -std=c++11 -O0
FLAGS		= -Wl,-rpath=. -Wall -Wextra -static-libstdc++
LIBS		= -lchipmunk -lsfml-graphics -lsfml-window -lsfml-system

SRCDIR		= src
FILES		= main.cpp Engine.cpp InputManager.cpp GInitState.cpp GMenuState.cpp GPlayState.cpp Character.cpp Terrain.cpp Camera.cpp AIController.cpp DataTypes.cpp Animation.cpp AnimatedSprite.cpp
SOURCES		= $(FILES:%.cpp=$(SRCDIR)/%.cpp)
EXEC		= Platformer

all : ${EXEC}

${EXEC} : ${SOURCES}
	${GCC} ${FLAGS} -o $@ $^ ${LIBS} 

clean :
	- rm ${EXEC}*
