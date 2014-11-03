#include <sstream>
#include <iostream>
#include "lua_comm.h"
#include "../Rendering/rendering.h"
#include "msgbox.h"
#include "../lock.h"
#include "../Progress.h"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

using namespace std;
//every component will have its own object
//each object is initialized by lua
//containing information such as:
//tasks it is associated with
//current animation
//interaction state should be updated per object rather than in progress

extern Progress game;
lua_State *mainL;
SDL_sem* msgLock;

int lua_comm::init_lua()
{
    L = lua_open();
    luaL_openlibs(L);
    luaopen_base(L);
    luaopen_table(L);
    luaopen_string(L);
    luaopen_math(L);
    //luaopen_package(L);
}

int lua_comm::close_lua()
{
    lua_close(L);
    L = NULL;
}

int l_addNPC(lua_State* l)
{
    SDL_SemWait(msgLock);
    char* npcpath = (char*)lua_tostring(mainL,-1); lua_pop(mainL,1);

    if (lua_gettop(mainL)>0)
    {
        int y = lua_tonumber(mainL,-1);lua_pop(mainL,1);
        int x = lua_tonumber(mainL,-1);lua_pop(mainL,1);
        game.gameScene[game.currentscene]->addComponent(npcpath,x,y);
    }
    else game.gameScene[game.currentscene]->addComponent(npcpath);
    lua_pop(mainL,1);
    SDL_SemPost(msgLock);
    return 0;
}

int lua_comm::execute(char* filepath)
{
        if (L == NULL) init_lua();
        printf("path = %s\n",filepath);
        int err = 0;
        try{
        err = luaL_dofile(L,filepath);
        }
        catch(...)
        {
            printf("error while lual_dofile\n");
        }
        if (err)
        {
            string str = lua_tostring(L, lua_gettop(L));
            printf("executelua Error:%s\n",str.c_str());
            lua_pop(L, 1);
        }
        printf("finished executing\n");
}
void lua_comm::getArray(string* str)
{
    lua_pushnil(L);
    int i = 0;
    while(lua_next(L, -2) != 0)
    {
        char* keyword= (char*)lua_tostring(L,-1);
        //initialize NPCs here
        for(int t = 0; keyword[t] != 0; t++)
                str[i] += keyword[t];
        i++;
        lua_pop(L, 1);
    }
    lua_pop(L,1);
}

int l_completeTasks(lua_State *l)
{
}

int init_luaMain()
{
    mainL = lua_open();
    luaL_openlibs(mainL);   luaopen_base(mainL);
    luaopen_table(mainL);  luaopen_string(mainL); luaopen_math(mainL);
    lua_register(mainL,"blit", l_blit);
    lua_register(mainL,"showMsg",l_showMSG);
    lua_register(mainL,"addV",l_addVariable);
    lua_register(mainL,"checkInt",l_checkInt);
    lua_register(mainL,"checkString",l_checkString);
    lua_register(mainL,"addNPC",l_addNPC);
    lua_register(mainL,"showMsgA",l_showMSGA);
    lua_register(mainL,"msgSelect",l_choose);
    lua_register(mainL,"msgSelectA",l_chooseA);
    lua_register(mainL,"setInt",l_setINT);
    lua_register(mainL,"enterScene",l_enterScene);
    msgLock = SDL_CreateSemaphore( 1 );
}

map<string,lua_Variable*> lua_Vs::v;

void lua_Vs::addV(string key)
{
    lua_Variable *vadd = new lua_Variable();
    //newV->type = type;
    vadd->value = new int(0);
    map<string,lua_Variable*>::iterator found = v.find(key);
    if (found==v.end()) v.insert(std::pair<string, lua_Variable*>(key,vadd));
}

int close_luaMain();
/////////Exposed C functions//////
int l_enterScene(lua_State *l)
{

    game.gameScene[game.currentscene]->tmpList.clear();
    printf("enter scene\n");
    char *sc = (char*) lua_tostring(mainL,-1);
    printf("scene = %s\n",sc);
    string* newstr = new string(sc);
//    cout << *newstr<<endl;
    game.currentscene = *newstr;
    game.gameScene[*newstr]->enterScene();

    //delete newstr;
    //deleting it it will cause the game to crash
    printf("done entering scene\n");
}

int l_setINT(lua_State *l)
{
    printf("before setting variable\n");
    int val = lua_tonumber(mainL,-1);lua_pop(mainL,1);
    char* name = (char*)lua_tostring(mainL,-1);
    printf("name = %s\n",name);
    string strn = string(name);
    delete[] name;
    lua_Vs::addV(strn); //create it if it doesn't already exist
    lua_pop(mainL,1);
    printf("now name = %s\n",strn.c_str());
    *(int*)lua_Vs::v[strn]->value = val;;
    printf("after setting variable\n");
    return 0;
}

int l_choose(lua_State *l)
{
    SDL_SemWait(msgLock);
    char* fontname = (char*)lua_tostring(mainL,-1);lua_pop(mainL,1);
    int y = lua_tonumber(mainL,-1);lua_pop(mainL,1);
    int x = lua_tonumber(mainL,-1); lua_pop(mainL,1);
    int n = lua_tonumber(mainL,-1); lua_pop(mainL,1);

    wstring* str = new wstring[n];
    for (int i = n-1; i >=0;i--)
    {
        wchar_t* tmp = (wchar_t*) lua_tostring(mainL,-1);
        str[i] = wstring(tmp);
        lua_pop(mainL,1);
    }

    wchar_t* m = (wchar_t*) lua_tostring(mainL,-1); lua_pop(mainL,1);
    //wstring msg = wstring(m);
    std::wstringstream buf;
    buf << m;

    wstring msg = buf.str();
    int res = msgbox::displayOptions(fontname,msg,str,n,x,y);
    lua_pushnumber(mainL,res);

    delete[] str;
    SDL_SemPost(msgLock);
    return 1;
}

int l_chooseA(lua_State *l)
{
    int y = lua_tonumber(mainL,-1);lua_pop(mainL,1);
    int x = lua_tonumber(mainL,-1); lua_pop(mainL,1);
    int n = lua_tonumber(mainL,-1);
    lua_pop(mainL,1);
    SDL_SemWait(msgLock);
    string* str = new string[n];
    for (int i = n-1; i >=0;i--)
    {
        char* tmp = (char*) lua_tostring(mainL,-1);
        str[i] = string(tmp);
        lua_pop(mainL,1);
    }

    char* m = (char*) lua_tostring(mainL,-1); lua_pop(mainL,1);
    string msg = string(m);

    int res = msgbox::displayOptions(msg,str,n,x,y);
    lua_pushnumber(mainL,res);
    SDL_SemPost(msgLock);
    return 1;
}

int l_addMSGA(lua_State *l)
{

}

int l_showMSGA(lua_State *l)
{
    char* fontname = (char*) lua_tostring(mainL,-1);lua_pop(mainL,1);
    char* ctxt = (char*) lua_tostring(mainL,-1);
    string* msg = new string(ctxt);

    int x = 30;
    int y = 30;
    /*if (lua_next(mainL,1)!=0)
    {
        y = lua_tonumber(mainL,-1);lua_pop(mainL,1);
        x = lua_tonumber(mainL,-1);lua_pop(mainL,1);
    }*/

    //option to take line1, line2 and line3
    //SDL_SemWait(msgLock);
    RenderText::drawText(fontname,*msg,x,y,true);
    //now wait for input

   // SDL_SemPost(msgLock);
     lua_pop(mainL,1);
    delete msg;
    return 0;
}
int l_blit(lua_State *l)
{
    #ifndef _DEBUG
    printf("BlitImg::begin\n");
    #endif
    SDL_SemWait(msgLock);
    char* img = (char*) lua_tostring(mainL,-1);
    //load image
    int y = lua_tonumber(mainL,-2);lua_pop(mainL,1);
    int x = lua_tonumber(mainL,-2);lua_pop(mainL,1);
    game.gameScene[game.currentscene]->addImg(img,x,y);
    lua_pop(mainL,1);
    SDL_SemPost(msgLock);

    //glBindTexture(GL_TEXTURE_2D, texture);
    //draw_texture(x, y, w, h,0, 0, w, h);
    //add texture, x,y,w,h to list
    //so that they get rendered
}

int l_showMSG(lua_State *l)
{
    //need a messagebox class
    //messagebox class will use a list to contain the messages
    //showMSG should be added to the list
    #ifndef _DEBUG
    printf("showMSG::begin\n");
    #endif
    //SDL_SemWait(msgLock);
    Rendering::RestoreScene();
     char* fontname = (char*) lua_tostring(mainL,-1); lua_pop(mainL,1);
    wchar_t* txt = (wchar_t *) lua_tostring(mainL,-1);
   // char* ctxt = (char*) lua_tostring(mainL,-1);


    std::wstringstream buf;
    buf << txt;
    lua_pop(mainL,1);


    int x = 30;
    int y = 30;
    if (lua_gettop(mainL)>0)
    {
        y = lua_tonumber(mainL,-1);lua_pop(mainL,1);
        x = lua_tonumber(mainL,-1);lua_pop(mainL,1);
    }
    //wstring msg = buf.str();

    //wstring drawmsg = processWide(*msg);
    #ifndef _DEBUG
    printf("showMSG::before RenderText\n");
    #endif
    RenderText::drawText(fontname,buf.str(),x,y,true);
   // delete msg;
    //SDL_SemPost(msgLock);
    printf("showMSG::done\n");

    //delete msg;
    return 0;
}


 int l_checkInt(lua_State *l)
{
    SDL_SemWait(msgLock);
    printf("before check int\n");
    char* name = (char*)lua_tostring(mainL,-1);
    string strn = string(name);
    delete[] name;

    lua_Vs::addV(strn); //create it if it doesn't already exist
    printf("after addv\n");
    int res = *(int*)lua_Vs::v[strn]->value;
    lua_pop(mainL,1);
    lua_pushnumber(mainL,res);

    printf("after check int:res = %d\n",res);
    SDL_SemPost(msgLock);
    return 1;
}
 int l_checkString(lua_State *l)
 {

 }
 int l_addVariable(lua_State *l)
{
    //get type
    //get name
    //#if DEBUG
    printf("lua_comm::addvariable\n");
    //#endif
    char type = (char) lua_tonumber(mainL,-1); lua_pop(mainL,1);
    char* name = (char *) lua_tostring(mainL,-1);

    printf("addvariable::name = %s\n",name);
    string* strn = new string(name);
    lua_Vs::addV(*strn);

    lua_pop(mainL,1);
    printf("lua_comm::added variable\n");
    //add to list, probably better with a hash table
    delete[] strn;
}
