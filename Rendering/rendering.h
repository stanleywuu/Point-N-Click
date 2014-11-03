#ifndef RENDERING_H_INCLUDED
#define RENDERING_H_INCLUDED

#include <string>
#include <map>
#include "../funcs/hashtable.h"
#include "../Scenes/scenes.h"

using namespace std;
class Rendering
{
    //the global rendering function
    public:
        static GLuint texture;
        static void RestoreScene();
        static scenes lastScene;
        static void loadFont(char* path);
//        void drawObject(GLuint text_obj,int x,int y,int rotate); //draw object on top of the texture

};


class RenderText
{
    public:
        static map<string,Font*> gameFonts;
        static void drawText(string font,string txt,int x,int y,bool wait);
        static void drawText(string font,wstring txt,int x,int y,bool wait);
        static void addMap (string key,  Font* font);
};


#endif // RENDERING_H_INCLUDED
