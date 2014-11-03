#include <string>
#include <list>
#include "lua_comm.h"
#include "Component.h"

#define NONE 0
#define SHAKE 1
#define FLOAT 2

using namespace std;
extern lua_State* mainL;

void Component::animate()
{
    switch (animation)
    {
        case SHAKE:
            rotation = rotation > 0? 0:rotate;
        break;
        case FLOAT:
            if (x > 0 && x < 800) x++;
            else x = 0;
        break;
        default:
        break;
    }
}

Component::~Component()
{
    if (surface!=NULL)
       SDL_FreeSurface(surface);
}

void Component::interact(int x,int y)
{

    lua_settop(mainL, 0);
   printf("begin interact\n");
    char* file = new char[strlen(path.c_str())+1];
    strcpy(file,path.c_str());


    luaL_dofile(mainL,file);

    printf("component::interact::beforesend\n");

    lua_getglobal(mainL,"interact");

    lua_pushnumber(mainL,x);
    lua_pushnumber(mainL,y);

   if (lua_pcall(mainL, 2, 0, 0) != 0)
          printf("error running function `f': %s",
     lua_tostring(L, -1));
    printf("interact\n");

    delete[] file;
    //lua_pop(mainL,1);
}

void Component::render()
{
   // animation = SHAKE;
    animate();
    //prep_texture(surface,texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glRotatef(rotation,0,0,1);
    draw_texture(x, y, w, h,0, 0, w, h);
    glRotatef(0-rotation,0,0,1);
}
