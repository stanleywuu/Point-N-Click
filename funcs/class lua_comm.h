#ifndef CLASS_LUA_COMM_H_INCLUDED
#define CLASS_LUA_COMM_H_INCLUDED

class lua_comm
{
    public:
        lua_State *L;
        int init_lua();
        int close_lua();
        virtual int execute(string filepath);

};

#endif // CLASS_LUA_COMM_H_INCLUDED
