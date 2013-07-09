#ifndef TERRAIN_H
#define TERRAIN_H

#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <vector>

class TUnit;

class Terrain
{
public:
    Terrain() { };
	Terrain(cpSpace *s);
	~Terrain();

    cpVect tileAtCoords(sf::Vector2f coords);
    std::vector<cpVect> getPath(cpVect tile1, cpVect tile2);

	void drawTerrain(sf::RenderWindow *window, sf::Transform trans);

private:
    cpSpace *space;
    int tileMap[64][64];

    int tileSize;
	//std::vector<TUnit*> terrain;
};

class TUnit
{
public:
	TUnit();
    ~TUnit();

    void addShape(cpShape *s);

	void draw(sf::RenderWindow *window);

    cpBody *body;
    sf::Vertex line[2];

private:
    std::vector<cpShape*> shapes;

    //sf::Texture texture;
    //sf::Sprite sprite;
};

#endif
