#ifndef CLASS_LUA_COMM_H_INCLUDED
#define CLASS_LUA_COMM_H_INCLUDED
#include <string>
#include <map>

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
using namespace std;

class lua_comm
{
    public:
        lua_State *L;
        int init_lua();
        int close_lua();
        int execute(char* filepath);
        void getArray(string* str);
};


struct lua_Variable
{
    char type;
    void* value;
};

class lua_Vs
{
    public:
        static map<string,lua_Variable*> v;
        static void addV(string key);;
};
 int l_blit(lua_State*l);
 int l_setINT(lua_State *l);
 int l_choose(lua_State *l);
 int l_chooseA(lua_State *l);
 int init_luaMain();
 int close_luaMain();
 int l_showMSG(lua_State *l);
 int l_enterScene(lua_State *l);
 int l_showMSGA(lua_State *l);
 int l_addVariable(lua_State *l);
 int l_checkInt(lua_State *l);
 int l_checkString(lua_State *l);
 int l_addNPC(lua_State *l);
#endif // CLASS_LUA_COMM_H_INCLUDED
