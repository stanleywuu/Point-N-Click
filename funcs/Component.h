#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED
#include "SDLOGL.h"
#include "lua_comm.h"

using namespace std;

class Component : public lua_comm
{
    public:
        string path;
        int id;
        int nextTask;
        int animation;
        string* tasks;
        char* img;
        void render(); //need to render on the right canvas/texture
        int ntasks;
        int rotate;
        int x;
        int y;
        int w;int h;
        GLuint texture;
        SDL_Surface* surface;

        void interact(int x,int y);


        ~Component();
    private:
        int rotation;
        void animate();
};


#endif // COMPONENT_H_INCLUDED
