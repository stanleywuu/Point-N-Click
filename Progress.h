#ifndef PROGRESS_H_INCLUDED
#define PROGRESS_H_INCLUDED
#include <list>
#include <map>
#include "Scenes\scenes.h"

using namespace std;

class Progress:public lua_comm
{
    public:
        void Init(char* path);
        void addNewTask(string task);
        void TaskComplete(string task);
        void advanceScene(string loc);
        list<string> keywords;

        int scenesn;
        string currentscene;
        map<string,scenes*> gameScene;
    private:
        string playerName;
        string state;
        string loc;
        string* sceneLoc;
        //stl list of all the tasks
        //hashlist of items
        list<string> tasksInProgress;
        list<string> tasksDone;
};


#endif // PROGRESS_H_INCLUDED
