#include "Component_Manager.h"

void Component_Manager::init_Array(int total)
{
    components = new Component[total]; nextEmpty = 0;
    for (int i = 0; i < total;i++) components[i].surface = NULL;
}
void Component_Manager::addComponent(Component *c)
{
    components[nextEmpty] = *c;
    nextEmpty++;
}
void Component_Manager::clear()
{
    if (nextEmpty > 0)  delete[] components;
    nextEmpty = 0;
}

Component* Component_Manager::findObj(int x,int y)
{
    Component* res = NULL;
        //if x and y falls within the square
        //check colours
        //only do this if we click
    for (int i = 0;i < nextEmpty;i++)
    {
        if (x > components[i].x && x < components[i].x + components[i].w)
        {
            if (y > components[i].y && y < components[i].y+components[i].h)
            {
                //res = new Component();
                //check if the spot there is right
                if(SDL_MUSTLOCK(components[i].surface))
                        SDL_LockSurface(components[i].surface);

                SDL_PixelFormat* fmt = components[i].surface->format;
                Uint32 *pixels = (Uint32 *)components[i].surface->pixels;
                //Get the requested pixel
                Uint32 pixel =  pixels[ ( (y- components[i].y) * components[i].surface->w ) + x - components[i].x ];

                Uint32 temp = pixel & fmt->Amask;
                temp = temp >> fmt->Ashift;
                temp = temp <<fmt->Aloss;

                if ((Uint8) temp > 0) res = &components[i];
            if(SDL_MUSTLOCK(components[i].surface))
                        SDL_UnlockSurface(components[i].surface);

            }
        }
    }
    return res;
}
void Component_Manager::returnComponent (int id){}
void Component_Manager::replaceComponent (int id, Component c){}
void Component_Manager::removeComponent(int id){}
void Component_Manager::RenderAll()
{
    int i = 0;
    while (i < nextEmpty)
    {
        components[i].render();
        i++;
    }
}
