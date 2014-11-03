#ifndef SCENES_H_INCLUDED
#define SCENES_H_INCLUDED

#include <iostream>
#include "..\funcs\SDLOGL.h"
#include "..\funcs\Component_Manager.h"

using namespace std;
class Component;
class Component_Manager;
class img
{
    public:
        SDL_Surface* surface;
        GLuint texture;
        int x,y,w,h;

};
class scenes: public lua_comm
{
    public:
        string* objects;
        string fpath;
        string name;
        void initScene(char* path);
        void InitObjects(char* path);
        void enterScene();
        void Render();
        void addComponent(char* path);
        void addComponent(char* path,int x,int y);
        Component* Interact(int x,int y);
        Component_Manager tmpList;
        void  addImg(char* path,int x,int y);

    private:
        void clearImg();
        list<img*> imgList;
        Component_Manager objList;
        int tasks;
        int width;
        int height;
        int nobjects;
        bool entered;

        string *taskstr;
        char* bgimg;
        char* music_path;
        SDL_Surface * bgsurface;
        GLuint bgtext;

        Component* loadComponent(char* path);

     //   Component_Manager components;
};

#endif // SCENES_H_INCLUDED
