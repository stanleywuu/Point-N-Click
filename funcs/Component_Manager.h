#ifndef COMPONENT_MANAGER_H_INCLUDED
#define COMPONENT_MANAGER_H_INCLUDED
#include <string>
#include <list>
#include "Component.h"

using namespace std;
class Component;

class Component_Manager
{
    public:
        void init_Array(int total); //we should use array
        void addComponent(Component *c);
        void returnComponent (int id);
        void replaceComponent (int id, Component c);
        void removeComponent(int id);
        void RenderAll();
        void clear();
        Component* findObj(int x,int y);
    private:
        int nextEmpty;
        Component * components;
};
#endif // COMPONENT_MANAGER_H_INCLUDED
