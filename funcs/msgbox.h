#ifndef MSGBOX_H_INCLUDED
#define MSGBOX_H_INCLUDED
#include "../Rendering/rendering.h"
#include <vector>
#include <string>
using namespace std;

struct msgInfo
{
    string txt;
    int x;
    int y;
};

class msgbox
{
    public:
        static void addMsg(string txt, int x,int y);
        static void showMsg();
        static int displayOptions(string msg, string* options,int n,int x,int y);
        static int displayOptions(char* fontname,wstring msg, wstring* options,int n,int x,int y);
    private:
        static int i;
        static int x;
        static int y;
        static vector<msgInfo> msg;
        static vector<string>::iterator it;
};
#endif // MSGBOX_H_INCLUDED
