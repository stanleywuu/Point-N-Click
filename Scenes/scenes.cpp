#include "..\funcs\lua_comm.h"
#include <iostream>
#include "scenes.h"
#include "..\Rendering\rendering.h"
#include "..\Global.h"

extern lua_State* mainL;

void scenes::Render()
{
    //bind background first and then draw objects
    //GLuint scenetext;
    //prep_texture(bgsurface,bgtext);
    glBindTexture(GL_TEXTURE_2D, bgtext);
    draw_texture(0, 0, Global::width, Global::height,0, 0, width, height);
    //draw_texture(0, 0, bgsurface->w, bgsurface->h,0, 0, bgsurface->w, bgsurface->h);
    objList.RenderAll();
    tmpList.RenderAll();

    list<img*>::iterator i;
    for(i=imgList.begin(); i != imgList.end(); ++i)
    {
    img* image = *i;
    glBindTexture(GL_TEXTURE_2D, image->texture);
    draw_texture(image->x, image->y, image->w, image->h,0, 0, image->w, image->h);
    }

    Rendering::lastScene = *this;
    SDL_GL_SwapBuffers();
    //scenetext = bgtext;
    //glBindTexture( GL_TEXTURE_2D, scenetext );
    //now we can draw stuff on top of it
}

Component* scenes::Interact(int x,int y)
{
    Component* res = NULL;
    res = objList.findObj(x,y);
    if (res == NULL) res = tmpList.findObj(x,y);
    return res;
}

void scenes::enterScene()
{
    tmpList.init_Array(10);
    clearImg();
    Mix_Music* music = Mix_LoadMUS(music_path);
    if(!music) {
    printf("Mix_LoadMUS:%s: %s\n", music_path,Mix_GetError());}
    // this might be a critical error...
    if (music!=NULL)
    {
        if( Mix_PlayMusic(music,-1 ) == -1 ) { printf("error playing music %s\n",music_path); }
    }
    Render();
    #ifndef _DEBUG
    printf("begin Entering Scene\n");
    #endif

    char* file = new char[strlen(fpath.c_str())+1];
    strcpy(file,fpath.c_str());

    luaL_dofile(mainL,file);
    delete[] file;
    #ifndef _DEBUG
    printf("SCENES::ENTER::Before Execute\n");
    #endif
    lua_getglobal(mainL,"Enter");

   if (lua_pcall(mainL, 0, 0, 0) != 0)
          printf("error running function `f': %s",
     lua_tostring(mainL, -1));
    printf("ENd of Enter Scene\n");
}
void scenes::clearImg()
{
    list<img*>::iterator i;
    for (i = imgList.begin();i!= imgList.end();++i)
    {
        delete *i;
    }
    imgList.clear();
}
void  scenes::addImg(char* path,int x,int y)
{
        img* newImg = new img;
        string filename = string(path);
        newImg->surface = loadimage(filename);
        if (newImg->surface!=NULL)
        {
        newImg->w = newImg->surface->w;newImg->h=newImg->surface->h;
        prep_texture(newImg->surface,newImg->texture);

        SDL_FreeSurface(newImg->surface);
        newImg->x = x;newImg->y = y;
        newImg->surface = NULL;
        imgList.push_back(newImg);
        }
        else printf("Error:could not load image %s\n",path);
        Render();
}

Component* scenes::loadComponent(char* path)
{
    Component* res = NULL;;
    //only keep going of lua is open
    printf("begin loadComponent:%s\n",path);

    res = new Component();
    execute(path);
    printf("executed script\n");
    lua_getglobal(L,"x"); int x = lua_tonumber(L,-1);
    lua_getglobal(L,"y"); int y = lua_tonumber(L,-1);
    lua_getglobal(L,"animation"); int animation = lua_tonumber(L,-1);
    lua_getglobal(L,"img"); char *img = (char *)lua_tostring(L,-1); //this line probably need to be changed
    lua_getglobal(L,"rotate"); int rotate = lua_tonumber(L,-1);
    lua_getglobal(L,"height"); int h = lua_tonumber(L,-1);
    lua_getglobal(L,"width"); int w = lua_tonumber(L,-1);
    res->x = x;res->y = y;
    res->img = img; //probably should be a copy

    printf("done loading component\n");

    res->path = string(path);
    res->surface = loadimage(img);
    res->h = h;
    res->w = w;
    prep_texture(res->surface,res->texture);
    //SDL_FreeSurface(res->surface);

    return res;
}

void scenes::addComponent(char* path,int x,int y)
{
    printf("begin adding component:%s\n",path);
    Component* c = loadComponent(path);
    c->x = x;c->y = y;
    printf("adding component:%s\n",path);
    if (c != NULL) tmpList.addComponent(c);
    Render();
    printf("Done component:%s\n",path);
}
void scenes::addComponent(char* path)
{
    printf("begin adding component:%s\n",path);
    Component* c = loadComponent(path);
    printf("adding component:%s\n",path);
    if (c != NULL) tmpList.addComponent(c);
    Render();
    printf("Done component:%s\n",path);
}

void scenes::initScene(char* path)
{
    //load the scene script
    fpath = string(path);
    entered = false;
    init_lua();
    execute (path);
    //upon entering a new scene, we need to do execute the init function in lua script
    //first example of doing things
    //we load all the tasks associated with this one

    lua_getglobal(L,"musicpath");

    char* mpath = (char*) lua_tostring (L,-1);
    music_path = new char[strlen(mpath)+1];

    for (int i = 0;i < strlen(mpath);i++)
    {
        music_path[i] = mpath[i];
    }
    music_path[strlen(mpath)] = '\0';

    lua_getglobal(L,"bg");
    bgimg = (char*) lua_tostring(L,-1);
    //strcpy(bgimg, (char*)lua_tostring(L,-1));
    //lua_getglobal(L,"tasks");
    //getArray(taskstr);
    //we got all the taskstr for entry, now we get all the scripts inside the objects
    //create objects based on

    #ifndef _DEBUG
    // code here only runs in debug mode
    printf("bgimg = %s\n",bgimg);
    #endif

    //now we do the same thing for the objects

    bgsurface = loadimage(bgimg);
    printf("after loading bgimg %s\n");
    width = bgsurface->w;
    height = bgsurface->h;
    printf("width = %d,height = %d\n",width,height);
    prep_texture(bgsurface,bgtext);
    printf("after prep_texture\n");
    //free_surface(bgsurface);
    //now bg_text should have the background img

    lua_getglobal(L,"nobjects");
    nobjects = lua_tonumber(L,-1);
    objects = new string[nobjects];

    printf("load objects\n");
    if (nobjects > 0)
    {lua_getglobal(L,"objects");
    getArray(objects);
    }

    objList.init_Array(nobjects);

    for (int ind = 0; ind < nobjects; ind++)
    {
        Component* c = loadComponent((char *)objects[ind].c_str());
        if (c != NULL) objList.addComponent(c);
    }
    SDL_FreeSurface(bgsurface);
    bgsurface = NULL;
    //and if one of the currently ongoing tasks matches with it, we will execute
    //take the response and execute it
    close_lua();
}
