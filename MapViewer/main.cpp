/********************************************************************/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <iostream>
#include <string>
using namespace std;

#include "base64.h"
#include "Timer.h"
#include "TMXLoader.h"

/********************************************************************/

//The screen sttributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;


SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

TMXLoader* tmxLoader = NULL;

/********************************************************************/

//void load_map_data();
void handle_input();
bool init();
void cleanup();
SDL_Surface *load_image( std::string filename );
void handle_movement();
void draw_everything();
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

/********************************************************************/
int main( int argc, char* args[] )

{

    //cout << "Hello";

    //load_map_data();

    //Start SDL
    //SDL_Init( SDL_INIT_EVERYTHING );

    // Display



    //Quit flag
    bool quit = false;

    //The dot that will be used
    //Dot myDot;

    //Keeps track of time since last rendering
    Timer delta;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    // TODO: Load files

    //Start delta timer
    delta.start();

    //While the user hasn't quit
    while( quit == false )
    {
        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the dot
            handle_input();

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        handle_movement();

        //Restart delta timer
        delta.start();

        //Fill the screen white
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

        draw_everything();

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    cleanup();


    return 0;
}
/********************************************************************/
//void load_map_data()
//{
//    std::string compressed = "AAAAAAEAAAABAAAAAAAAAAAAAAAAAAAAAQAAAAEAAAABAAAA";
//    string unencodedstr = base64_decode(compressed);
//    int LENGTH = unencodedstr.length();
//    const char* unencoded = unencodedstr.c_str();
//
//    for (int i = 0; i < LENGTH; i += 4)
//    {
//        int gid = unencoded[i] |
//              unencoded[i + 1] << 8 |
//              unencoded[i + 2] << 16 |
//              unencoded[i + 3] << 24;
//
//        cout << gid << " - ";
//
//        if ((i + 4) % 12 == 0) {
//
//            cout << endl;
//        }
//    }
//}
/********************************************************************/
void handle_input()
{
//    //If a key was pressed
//    if( event.type == SDL_KEYDOWN )
//    {
//        //Adjust the velocity
//        switch( event.key.keysym.sym )
//        {
////            case SDLK_UP: yVel -= DOT_VEL; break;
////            case SDLK_DOWN: yVel += DOT_VEL; break;
////            case SDLK_LEFT: xVel -= DOT_VEL; break;
////            case SDLK_RIGHT: xVel += DOT_VEL; break;
//        }
//    }
//    //If a key was released
//    else if( event.type == SDL_KEYUP )
//    {
//        //Adjust the velocity
//        switch( event.key.keysym.sym )
//        {
////            case SDLK_UP: yVel += DOT_VEL; break;
////            case SDLK_DOWN: yVel -= DOT_VEL; break;
////            case SDLK_LEFT: xVel += DOT_VEL; break;
////            case SDLK_RIGHT: xVel -= DOT_VEL; break;
//        }
//    }
}
/********************************************************************/
bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "TMX Map Viewer", NULL );



    tmxLoader = new TMXLoader();

    tmxLoader->loadDocument();


    //If everything initialized fine
    return true;
}
/********************************************************************/
void cleanup()
{
    delete tmxLoader;

    //Quit SDL
    SDL_Quit();
}
/********************************************************************/
SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}
/********************************************************************/
void handle_movement()
{



}
/********************************************************************/
void draw_everything()
{
    //SDL_Surface* tileset = tmxLoader->getTilesetImage();
    SDL_Surface* tileset = tmxLoader->getMapImage();


    if (tileset != NULL) {
        int x = 0;
        int y = 0;
        apply_surface( x, y, tileset, screen, NULL );
    }
}
/********************************************************************/

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

