#include "msgbox.h"
#include "SDLOGL.h"

int msgbox::i;
int msgbox::x;
int msgbox::y;
vector<msgInfo> msgbox::msg;
vector<string>::iterator msgbox::it;



int msgbox::displayOptions(string msg, string* options,int n,int x,int y)
{
    int res = -1;
    RenderText::drawText("123",msg,x,y,false);
    //now draw a box of size from x, y with height of n * 32
    for (int i = 1;i <= n;i++)
    {
        RenderText::drawText("123",options[i-1],x,y+i*32,false);

    }
    SDL_Event e;
    bool done = false;
    SDL_GL_SwapBuffers();
    while (SDL_PollEvent(&e) || !done)
        {
            // check for messages
            switch (e.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    //this is where a decision is made
                    int mx = e.motion.x;int my = e.motion.y;
                    if (mx > x && my > y + 32 && my < y + 32 * (1+n))
                    {
                        res = (my - y) / 32 - 1;
                        done = true;
                    }
                }
            case SDL_MOUSEMOTION:
                {
                    //do nothing for now, draw a yellow box around later
                }
            }
        }
    //now wait for selection

        Rendering::RestoreScene();
    return res;
}


int msgbox::displayOptions(char* fontname,wstring msg, wstring* options,int n,int x,int y)
{
    int res = -1;

    RenderText::drawText(fontname,msg,x,y,false);
    //now draw a box of size from x, y with height of n * 32
    for (int i = 1;i <= n;i++)
    {
        RenderText::drawText(fontname,options[i-1],x,y+i*32,false);

    }
    SDL_Event e;
    bool done = false;
    SDL_GL_SwapBuffers();
    while (SDL_PollEvent(&e) || !done)
        {
            // check for messages
            switch (e.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    //this is where a decision is made
                    int mx = e.motion.x;int my = e.motion.y;
                    if (mx > x && my > y + 32 && my < y + 32 * (1+n))
                    {
                        res = (my - y) / 32 - 1;
                        done = true;
                    }
                }
            case SDL_MOUSEMOTION:
                {
                    //do nothing for now, draw a yellow box around later
                }
            }
        }
    //now wait for selection

    Rendering::RestoreScene();
    return res;
}

void msgbox::addMsg(string txt,int x,int y)
{
    msgInfo info;
    info.txt = txt; info.x = x;info.y = y;
    msg.push_back(info);
    i = 0;
}

void msgbox::showMsg()
{
    //draw msg.at(i).txt at x and y
    if (i < msg.size())
    {
        msgInfo m = msg.at(i);
        RenderText::drawText("123",m.txt,m.x,m.y,true);
        i++;
    }
    if (i == msg.size()) msg.clear();
}
