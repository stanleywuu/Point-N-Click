#ifndef FREETYPE_H_INCLUDED
#define FREETYPE_H_INCLUDED

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <SDL/SDL_opengl.h>
#include <iostream>
#include <string>

using std::string;
//void drawtext(const char* txt, int size, int x,int y);

class FT2
{
      public:
          int init(char* fname, int width, int height);
          void loadchar(char ch, int x,int y);
          int adjx;
          int adjy;

          ~FT2();

      private:
          FT_Library ftlib;
          FT_Face face;


      int nextp2(int n);
};

int len(char* txt);
void drawtext(char* txt, int size, int x,int y);
void drawtext(string txt, int size,int tx,int ty);
#endif // FREETYPE_H_INCLUDED
