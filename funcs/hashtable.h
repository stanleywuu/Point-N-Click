#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED
#include <list>
#include "lua_comm.h"
#include "SDLOGL.h"

using namespace std;

struct Fontinfo
{
    public:
    int keycode;
    int x; int y; int w;int h;
};

class hashtable
{
    public:
        list<Fontinfo>* hashlist;
        void init(int size);
        void add(int key, Fontinfo f);
        Fontinfo returnFont(int key);
    private:
        int resolveKey(int key);
        int modkey;
};

class Font:lua_comm
{
    public:
        char* img;
        char name[40];
        //SDL_Surface* surface
        //GLuint texture
        int imagew,imageh;
        hashtable fontList;
        void drawString(wstring txt,int x,int y);
        void drawString(string txt,int x,int y);
        void readFontInfo(char* path);
        SDL_Surface* fsurface;
        GLuint ftexture;
        void renderText(int key, int x,int y,int w,int h);
        string returnLines(string txt,int max);
        wstring returnLines(wstring txt,int max);
    private:
//        void renderText(int key, int x,int y,int w,int h);
        //draw on texture
        //void draw(int key, int x,int y);
};
#endif // HASHTABLE_H_INCLUDED
