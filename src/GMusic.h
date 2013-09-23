#ifndef GMUSIC_H
#define GMUSIC_H

#include <SFML/Audio.hpp>
#include <vector>

class GMusic
{
    public:
        GMusic();
        ~GMusic();

        void play();
        void stop();
        void next();
        void update();

    private:
        bool playing;
        int songnum;
        std::vector<sf::Music*> songs;
};

#endif
