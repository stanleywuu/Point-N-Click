#include "hashtable.h"
#include <sstream>
#include <fstream>

void hashtable::init(int size)
{
    hashlist = new list<Fontinfo>[size];
    modkey = size;
}

void hashtable::add(int key, Fontinfo f)
{
    int slot = resolveKey(key);
    hashlist[slot].push_front(f);
}

Fontinfo hashtable::returnFont(int key)
{
        int slot = resolveKey(key);
        list<Fontinfo>::iterator it;
         for ( it=hashlist[slot].begin(); it != hashlist[slot].end(); it++ )
         {
             Fontinfo f = *it;
            if (f.keycode == key) return f;
         }
        //return empty
        Fontinfo empty;
        empty.keycode = -1;
        return empty;
}

int hashtable::resolveKey(int key)
{
    return key/modkey;
}

void Font::drawString(string txt,int x,int y)
{
    const char* ctxt = txt.c_str();
    //& with 0x 128
    unsigned int keycode = 0;
    int i = 0;
    int width = 0;
    while (ctxt[i]!='\0')
    {
        if (ctxt[i]=='\n' && ctxt[i+1]!='\0') {y+=32;i++;width = 0;}
        Fontinfo f = fontList.returnFont(ctxt[i]);
        width+=f.w;
        renderText(ctxt[i],x + width,y,32,32);
        i++;
    }
}
void Font::drawString(wstring txt,int x,int y)
{
    const wchar_t* ctxt = txt.c_str();
    //& with 0x 128
    int width = 0;
    int i = 0;
    printf("drawString::begin\n");
    while ((ctxt[i] & 0xFF00)!='\0')
    {
        //printf("ctxt[i] = %s\n",ctxt[i]);
        if (ctxt[i]=='\n' && (ctxt[i+1] & 0xFF00)!='\0')
        {y+=32;i++;width = 0;}

        Fontinfo f = fontList.returnFont(ctxt[i]);
        printf("before rendertext\n");
        renderText(ctxt[i],x + width,y,32,32);
        printf("after rendertext\n");
        width+=f.w;
        i++;
    }
    printf("drawString::end\n");
}


void Font::renderText(int key, int x,int y,int w,int h)
{
        //render a single text
        Fontinfo f = fontList.returnFont(key);
        if (f.keycode!=-1){
        glBindTexture(GL_TEXTURE_2D, ftexture);
        //draw_textureInt(x,y,f.x,f.y,imagew,imageh);
        //draw_textureInt(x,y,f.x,f.y,imagew,imageh);
        draw_textureInt(x,y,f.w,f.h ,f.x, f.y, imagew, imageh);}
}

wstring Font::returnLines(wstring txt,int max)
{
    std::wstringstream buf;
    wstring res = L"";
    printf("returnLines::beg\n");
    int width = 0;
    //go through the string, see if it goes over, if it does
    //make it a new line
    const wchar_t* ctxt = txt.c_str();
    int ind = 0;
    int l = txt.length();
    for (ind = 0;ind < l;ind++)
    {
        wchar_t c = ctxt[ind];
        printf("returnLines::printing ctxt[i]\n");
        printf("returnLines::key = %d\n",ctxt[ind]);
        Fontinfo f = fontList.returnFont(ctxt[ind]);
        width += f.w;
        //going over
        if (width > max)
        {
            buf << "\n";
            width = 0;
        }
      buf << c;
    }
    res = buf.str();
    printf("returnLines::end\n");
    return res;
}

string Font::returnLines(string txt,int max)
{
    std::stringstream buf;
    string res = "";
    printf("returnLines::beg\n");
    int width = 0;
    //go through the string, see if it goes over, if it does
    //make it a new line
    const char* ctxt = txt.c_str();
    int ind = 0;
    int l = txt.length();
    for (ind = 0;ind < l;ind++)
    {
        char c = ctxt[ind];
        printf("returnLines::printing ctxt[i]\n");
        printf("returnLines::key = %d\n",ctxt[ind]);
        Fontinfo f = fontList.returnFont(ctxt[ind]);
        width += f.w;
        //going over
        if (width > max)
        {
            buf << "\n";
            width = 0;
        }
      buf << c;
    }
    res = buf.str();
    printf("returnLines::end\n");
    return res;
}

void Font::readFontInfo(char* path)
{
    init_lua();
    execute(path);
    int nChars;
    //get name from there too so I can add it to renderText::gamefonts
    lua_getglobal(L,"fontname"); char* fname = (char*) lua_tostring(L,-1);
    lua_getglobal(L,"img"); char* img = (char*)lua_tostring(L,-1);
    lua_getglobal(L,"nchars"); nChars = lua_tonumber(L,-1);

    strcpy(this->name,fname);
    fsurface = loadimage(img);
    imagew = fsurface->w; imageh = fsurface->h; //record size
    prep_texture(fsurface,ftexture);
    //free_surface(surface);
    fontList.init(nChars);

    lua_getglobal(L,"chars");
    lua_pushnil(L);

    while (lua_next(L,-2)!=0)
    {
        Fontinfo f;
        f.keycode = lua_tonumber(L,-1); lua_pop(L, 1); lua_next(L,-2);
        f.x = lua_tonumber(L,-1);lua_pop(L,1); lua_next(L,-2);
        f.y = lua_tonumber(L,-1);lua_pop(L,1); lua_next(L,-2);
        f.w = lua_tonumber(L,-1);lua_pop(L,1); lua_next(L,-2);
        f.h = lua_tonumber(L,-1);lua_pop(L,1);
        fontList.add(f.keycode,f);
    }
    lua_pop(L,1);

    close_lua();
}
