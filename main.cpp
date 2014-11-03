#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif
#include "funcs/SDLOGL.h"
#include "funcs/lua_comm.h"
#include "Progress.h"
#include "funcs/hashtable.h"
#include "Rendering/rendering.h"
#include "Global.h"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

Progress game;// = new Progress();

int main ( int argc, char** argv )
{
    // initialize SDL video
    Global::height = 600;
    Global::width = 800;

    init( Global::width , Global::height);

    game.Init("data\\startup.lua");
    ////////////////////////////////////////////////
    Rendering::loadFont("data\\fonts.lua");
    //Font* newFont = new Font();
    //newFont->readFontInfo("data\\font.lua");
    //RenderText::addMap(newFont->name,newFont);
    /////////////////////////////////////////
    init( Global::width , Global::height);
    //initialize game info
    init_luaMain();
    //Progress game;
    game.Init("data\\startup.lua");

    scenes* currentScene = (game.gameScene[game.currentscene]);

    currentScene->enterScene();
    GLuint texture;
    // load an image
    //after making the background, we have to start throwing objects there
    //depending on the file
    //obj loc, cover img
    //Use a thread to determine where the mouse clicked
    //Make this an array, so we go through each element in array, check the x, y, w and h
    //

    // centre the bitmap on screen

    //bind texture then we can draw it

    //draw_texture(0, 0, 800, 600,0, 0, 800, 600);


    // program main loop
    bool done = false;

    while (!done)
    {
        // message processing loop
        SDL_Event event;
        currentScene = (game.gameScene[game.currentscene]);
        while (SDL_PollEvent(&event))
        {
         //   currentScene = (game.gameScene[game.currentscene]);
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    currentScene->Render();
                   Component* res = currentScene->Interact(event.motion.x,event.motion.y);

                    if (res!=NULL)
                    {
                        res->interact(event.motion.x,event.motion.y);
                        res = NULL;
                    }
                    break;
                }
            // check for keypresses
            case SDL_KEYDOWN:
                {
                    currentScene->Render();

                    //RenderText::drawText("123",L"鼠标模式",30,30);
                    //newFont->drawString(L"鼠标模式",30,30);
                    //newFont->renderText(35,30,30,20,20);
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            } // end switch
            SDL_GL_SwapBuffers();
        } // end of message processing

    } // end main loop

    // free loaded bitmap
    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
