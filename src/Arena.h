#ifndef TERRAIN_H
#define TERRAIN_H

#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <vector>

class Arena
{
public:
    Arena() { };
	Arena(cpSpace *s);
	~Arena();

	void drawArena(sf::RenderWindow *window, sf::Transform trans);

private:
    cpSpace *space;

    sf::Texture *texture;
    sf::Sprite sprite;
};

#endif
