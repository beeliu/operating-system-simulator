/*
    Belinda Liu 
    Implementation file for PCB class
*/

#include "pcb.h"

// constructor
PCB::PCB(int p_id)
{
    pid = p_id;
}

int PCB::get_pid()
{
    return pid;
}


void PCB::set_parent(int par)
{
    parent = par;
}

void PCB::add_child(int ch)
{
    children.push_back(ch);
}

void PCB::remove_child(int ch)
{
    bool found = false;
    int index = 0;
    for (int i = 0; i < children.size(); i++)
    {
	if (children[i] == ch)
	{
	    found = true;
	    index = i;
	}
    }
    if (found)
    {
	children.erase(children.begin() + index);
    }
}

int PCB::get_parent()
{
    return parent;
}

std::vector<int> PCB::get_children()
{
    return children;
}
