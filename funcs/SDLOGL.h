#ifndef SDLOGL_H_INCLUDED
#define SDLOGL_H_INCLUDED

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_Image.h>
#include <SDL/SDL_opengl.h>
#include "SDL/SDL_ttf.h"
#include <SDL/SDL_mixer.h>


using namespace std;
int nextp2(int n);
int init(int width, int height);
void draw_texture(float x, float y, float width, float height, int tx, int ty, int iwidth, int iheight);
void draw_textureInt(float x, float y, float width, float height, int tx, int ty, int iwidth, int iheight);
SDL_Surface *loadimage(std::string filename);

void prep_texture(SDL_Surface *surface, GLuint &texture);


#endif // SDLOGL_H_INCLUDED
