GCC			= g++ -std=c++11 -O0
FLAGS		= -Wl,-rpath=. -Wall -Wextra -static-libstdc++ -DSFML_STATIC
LIBS		= -lchipmunk -lsfml-graphics -lsfml-window -lsfml-system

SRCDIR		= src
OBJDIR		= obj
FILES		= main.cpp Engine.cpp InputManager.cpp GMenuState.cpp GCharacterSelectState.cpp GPlayState.cpp Character.cpp Fighter.cpp Shooter.cpp Ninja.cpp Bullet.cpp Arena.cpp HUD.cpp Animation.cpp AnimatedSprite.cpp
SOURCES		= $(FILES:%.cpp=$(SRCDIR)/%.cpp)
OBJECTS		= $(FILES:%.cpp=$(OBJDIR)/%.o)
EXEC		= 10_Seconds

all : ${EXEC}

${EXEC} : ${OBJECTS}
	${GCC} ${FLAGS} -o $@ $^ ${LIBS} 

${OBJDIR}/%.o : ${SRCDIR}/%.cpp
	${GCC} ${FLAGS} -c $< -o $@

clean :
	- rm ${EXEC}*
