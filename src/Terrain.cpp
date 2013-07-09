#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <vector>
#include <list>
#include <cmath>

#include "Terrain.h"

Terrain::Terrain(cpSpace *s)
{
    space = s;
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 64; j++) {
            if(i == 0 || j == 0 || i == 63 || j == 63)
                tileMap[i][j] = 2;
            else
                tileMap[i][j] = 1;
        }
    }
}

cpVect Terrain::tileAtCoords(sf::Vector2f coords)
{
    coords.x -= 16.f;
    coords.y -= 16.f;
    coords /= 32.f;
    return cpv(rint(coords.x), rint(coords.y));
}

float dist(cpVect a, cpVect b)
{
    return cpvlength(a + b);
}

struct Node
{
    float gScore;
    float fScore;

    cpVect cameFrom;
    bool visited;
};

bool isInSet(std::list<cpVect> list, cpVect v)
{
    for (std::list<cpVect>::const_iterator i = list.begin(), e = list.end(); i != e; ++i) {
        if(v == *i)
            return true;
    }
    return false;
}

std::vector<cpVect> reconstructPath(Node nodes[][64], cpVect start)
{
    int x = (int)start.x;
    int y = (int)start.y;
    if(nodes[x][y].cameFrom == start)
        return std::vector<cpVect>(1, start);

    std::vector<cpVect> path = reconstructPath(nodes, nodes[x][y].cameFrom);
    path.push_back(start);
    return path;
    
}

std::vector<cpVect> Terrain::getPath(cpVect tile1, cpVect tile2)
{
    std::list<cpVect> closedSet;
    std::list<cpVect> openSet;
    Node nodes[64][64] = {{}};
    /*int xOffset = (tile2.x - tile1.x) / 2;
    int x;
    if(tile1.x < tile2.x)
        x = tile1.x - xOffset;
    else
        x = tile2.x + xOffset;

    if(x < 0) x = 0;
    else if(x > 64) x = 64;

    int yOffset = (tile2.y - tile1.y) / 2;
    int y;
    if(tile1.y < tile2.y)
        y = tile1.y - yOffset;
    else
        y = tile2.y + yOffset;

    if(y < 0) y = 0;
    else if(y > 64) y = 64;*/
    
    nodes[(int)tile1.x][(int)tile1.y].gScore = 0;
    nodes[(int)tile1.x][(int)tile1.y].fScore = dist(tile1, tile2);
    nodes[(int)tile1.x][(int)tile1.y].cameFrom = tile1;
    openSet.push_back(tile1);
    
    while(!openSet.empty()) {
        cpVect current = openSet.front();
        for(std::list<cpVect>::const_iterator i = openSet.begin(), e = openSet.end(); i != e; ++i) {
            if(nodes[(int)current.x][(int)current.y].fScore > nodes[(int)i->x][(int)i->y].fScore)
                current = *i;
        }

        //printf("current tile - (%g, %g)\n", current.x, current.y);
        if(current == tile2) {
            return reconstructPath(nodes, tile2);
        }

        openSet.remove(current);
        nodes[(int)current.x][(int)current.y].visited = true;

        for(int i = -1; i < 2; i++) {
            for(int j = -1; j < 2; j++) {
                if(j == 0 && i == 0) continue;

                cpVect neighbor = cpv(current.x + i, current.y + j);
                if(neighbor.x < 0 || neighbor.x > 63 || neighbor.y < 0 || neighbor.y > 63)
                    continue;

                //printf("neighbor tile - (%g, %g)\n", neighbor.x, neighbor.y);

                if(tileMap[(int)neighbor.x][(int)neighbor.y] == 2)
                    nodes[(int)neighbor.x][(int)neighbor.y].visited = true;

                float gScore = nodes[(int)current.x][(int)current.y].gScore +
                                dist(current, neighbor);

                if(nodes[(int)neighbor.x][(int)neighbor.y].visited
                        && gScore >= nodes[(int)neighbor.x][(int)neighbor.y].visited)
                    continue;

                bool inSet = isInSet(openSet, neighbor);
                if(!inSet || gScore < nodes[(int)neighbor.x][(int)neighbor.y].gScore) {
                    nodes[(int)neighbor.x][(int)neighbor.y].cameFrom = current;
                    nodes[(int)neighbor.x][(int)neighbor.y].gScore = gScore;
                    nodes[(int)neighbor.x][(int)neighbor.y].fScore = gScore + dist(neighbor, tile2);
                    if(!inSet)
                        openSet.push_back(neighbor);
                }
            }
        }
    }
    return std::vector<cpVect>();
}

void Terrain::drawTerrain(sf::RenderWindow *window, sf::Transform trans)
{
    sf::RectangleShape box(sf::Vector2f(28, 28));
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 64; j++) {
            switch(tileMap[i][j]) {
                case 1:
                    box.setFillColor(sf::Color(64, 255, 64));
                    break;
                case 2:
                default:
                    box.setFillColor(sf::Color(255, 64, 128));
                    break;
            }
            box.setPosition(i * 32 + 1, j * 32 + 1);
            window->draw(box, trans);
        }
    }
}

Terrain::~Terrain()
{
}

//-------------------------------------------
//--------------- TUnit Class ---------------
//-------------------------------------------

TUnit::TUnit()
{
}

TUnit::~TUnit()
{
    cpBodyFree(body);
    for(cpShape *s : shapes) {
        cpShapeFree(s);
    }
}

void TUnit::addShape(cpShape *s)
{
    shapes.push_back(s);
}

void TUnit::draw(sf::RenderWindow *window)
{
    window->draw(line, 2, sf::Lines);
}
