#include "rendering.h"
#include "..\funcs\SDLOGL.h"

map<string,Font*> RenderText::gameFonts;
GLuint Rendering::texture;
scenes Rendering::lastScene;


wstring processWide(wstring msg)
{
    wstring txt = msg;
    for (int i = 0;i < txt.length();i++)
    {
        wchar_t t = txt[i];
        txt[i] = ((t << 8) + ((t & 0xFF00) >> 8));
    }
    return txt;
}

 void RenderText::drawText(string font, wstring txt,int x,int y,bool wait)
{
    int width = 0;
    Font* f = gameFonts[font];
    int maxwidth = 550;
    int length = txt.length();

    wstring copy = processWide(txt);
    wstring draw = f->returnLines(copy,maxwidth);
    printf("rendertext::AfterreturnLines\n");
    f->drawString(draw,x,y);
    if (!wait) return; //no need to wait
    //////////////////////////////////
    SDL_Event e;
    bool done = false;
    SDL_GL_SwapBuffers();
    while (SDL_PollEvent(&e) || !done)
        {   // check for messages
            switch (e.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                {
                   done = true;break;
                }
            case SDL_KEYUP:
                {done = true;break;}
            }
        }
        //restore
        printf("done waiting\n");
        Rendering::RestoreScene();
        printf("drawText(unicode)::after restore\n");
}

void Rendering::loadFont(char* path)
{
    lua_State* L;
    L = lua_open();     luaL_openlibs(L);
    luaopen_base(L);     luaopen_table(L);
    luaopen_string(L);     luaopen_math(L);
    luaL_dofile(L,path);

    lua_getglobal(L,"fontfiles");
    lua_pushnil(L);
    int i = 0;
    while(lua_next(L, -2) != 0)
    {
        char* fpath = (char*)lua_tostring(L,-1);
        //initialize NPCs here
        Font* newFont = new Font();
        newFont->readFontInfo(fpath);
        RenderText::addMap(newFont->name,newFont);
        lua_pop(L, 1);
    }
    lua_pop(L,1);
}

void Rendering::RestoreScene()
{
    lastScene.Render();
 SDL_GL_SwapBuffers();
}
//int* empty(){int* res = new int(5); for (int i = 0;i < 25;i++);return res;};
void RenderText::drawText(string font, string txt,int x,int y,bool wait)
{
    int width = 0;
    Font* f = gameFonts[font];
    //get the text width and separate them into multiple lines
    int maxwidth = 550;
    int length = strlen(txt.c_str());
    #ifndef _DEBUG
    printf("rendertext::beforereturnLines=%s\n",txt.c_str());
    #endif
    //int* res = empty();
    //string* draw = f->returnLines(txt,maxwidth);

    string copy (txt);
    #ifndef _DEBUG
    printf("rendertext::beforefree\n");
    #endif
    string draw = f->returnLines(copy,maxwidth);
    #ifndef _DEBUG
    printf("rendertext::AfterreturnLines",txt.c_str());
    #endif
    f->drawString(draw,x,y);

    //save current screen///////////////////////////////////////
    printf("rendertext::drawtext::txt=%s\n",txt.c_str());
    if (!wait)return;
    ////////////////////////////////////////////////////////////
    SDL_Event e;
    bool done = false;
    SDL_GL_SwapBuffers();
    while (SDL_PollEvent(&e) || !done)
        {   // check for messages
            switch (e.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                {
                   done = true;break;
                }
            case SDL_KEYUP:
                {done = true;break;}
            }
        }
        //restore
        Rendering::RestoreScene();
        printf("rendertext::drawtext::proceed\n");
}

 void RenderText::addMap(string key, Font* f)
{
    gameFonts.insert(std::pair<string, Font*>(key,f));
}
