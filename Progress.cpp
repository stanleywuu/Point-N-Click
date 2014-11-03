#include <string>
#include <list>
#include <iostream>

#include "funcs/lua_comm.h"
#include <cstring>
#include "Progress.h"
#include "Global.h"

using namespace std;

void Progress::addNewTask(string task)
{
    tasksInProgress.push_front(task);
}
void Progress::TaskComplete(string task)
{

}
//string Progress::iterateTask(int i)
//{

//}
//progress initializes scenes
//scenes initialize components upon first entry
int Global::height;
int Global::width;

void Progress::Init(char* path)
{
    init_lua();
    execute (path);
    //from executing path it should add all the scripts
    //number of scenes
    lua_getglobal(L,"scenes");
    scenesn = lua_tonumber(L,-1);

    lua_getglobal(L,"width");
    int width = lua_tonumber(L,-1);
    if (width > 800 && width < 10000) Global::width = width;

    lua_getglobal(L,"height");
    int height = lua_tonumber(L,-1);
    if (height > 600 && height < 10000) Global::height = height;

    sceneLoc = new string[scenesn];

    lua_getglobal(L,"initialscene");
    char* initialscene = (char *)lua_tostring(L,-1);

    currentscene = string(initialscene);;

    lua_getglobal(L,"initkey");
    char* key = (char*) lua_tostring(L,-1);
    string tkey = string(key);

    keywords.push_back(tkey);

    lua_getglobal(L,"folders");

    getArray(sceneLoc);
    close_lua();

    for (int t = 0; t < scenesn; t++)
    {   string initPath = "data\\" + sceneLoc[t] + "\\init.lua";
        char *pline = new char[initPath.size() + 1];

        strcpy ( pline, initPath.c_str() );

        scenes* newScene = new scenes();
        newScene->initScene(pline);
        newScene->name = sceneLoc[t];
        gameScene.insert(std::pair<string, scenes*>(sceneLoc[t],newScene));

        delete pline;
    }
    #ifndef _DEBUG
    // code here only runs in debug mode
    printf("sceneLoc initialization\n");
    for (int j = 0;j < scenesn;j++ )
    {
        cout << sceneLoc[j] <<endl;
    }
    #endif

    //initialize scenes
    //enter scenes[current]
}
