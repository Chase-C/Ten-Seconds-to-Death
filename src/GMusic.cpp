#include <stdio.h>
#include <stdlib.h>
#include "GMusic.h"

GMusic::GMusic()
{
    playing = false;
    songnum = rand() % 3;;
    for(int i = 0; i < 3; i++)
        songs.push_back(new sf::Music());

    if(!songs.at(0)->openFromFile("rec/BlazeOn.ogg")) 
        fprintf(stderr, "could not open file 'BlazeOn.ogg'\n");

    if(!songs.at(1)->openFromFile("rec/InertExponent.ogg")) 
        fprintf(stderr, "could not open file 'InertExponent.ogg'\n");

    if(!songs.at(2)->openFromFile("rec/KiloByte.ogg")) 
        fprintf(stderr, "could not open file 'KiloByte.ogg'\n");

    songs.at(0)->setVolume(40);
    songs.at(1)->setVolume(40);
    songs.at(2)->setVolume(40);
}

GMusic::~GMusic() { };

void GMusic::play()
{
    if(songs.at(songnum)->getStatus() != sf::Music::Playing) {
        playing = true;
        songnum = rand() % 3;
        songs.at(songnum)->play();
    }
}

void GMusic::stop()
{
    playing = false;
    songs.at(songnum)->stop();
}

void GMusic::next()
{
    int temp;
    do {
        temp = rand() % 3;
    } while(temp == songnum);

    songs.at(songnum)->stop();
    songs.at(temp)->play();
    songnum = temp;
}

void GMusic::update()
{
    if(playing && songs.at(songnum)->getStatus() != sf::SoundSource::Playing)
        next();
}
