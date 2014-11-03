#ifndef SDL_TTF_H_INCLUDED
#define SDL_TTF_H_INCLUDED

#include "SDLOGL.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_TTF.h>
#include <string>

class SDL_TTF
{
    public:
      SDL_TTF(int size);
            SDL_TTF();

      int prep_surface();
      int rendertext(std::string text,int x,int y);
      void setcolor(int red,int green,int blue);
      void setSize(int lsize){size=lsize;}
        GLuint font_texture;
      void free();
       int size;
    private:
      TTF_Font *font;
      SDL_Surface *message ;
       SDL_Color textColor;

};
#endif // SDL_TTF_H_INCLUDED
