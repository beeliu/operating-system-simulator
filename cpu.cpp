/* 
    Belinda Liu
    Implementation file for CPU class
*/

#include "cpu.h"

CPU::CPU()
{
    ready_queue; // empty ready_queue
    current = 1; // set initially running process to pid 1
}

void CPU::add(int p_id)
{
    // if nothing is in CPU (current == -1), put as currently running process
    if(current == 1)
    {
	current = p_id;
    }
    else
    {
        ready_queue.push_back(p_id);
    }
}

int CPU::get_running()
{
    return current;
}

std::vector<int> CPU::get_all()
{
    return ready_queue;
}

int CPU::remove(int p_id)
{
    // check for process in queue
    int found = -1;
    for (int i = 0; i < ready_queue.size(); i++)
    {
	if (ready_queue[i] == p_id)
        {
	    found = i;
	}
    }
    if (found >= 0)
    {
	ready_queue.vector::erase(ready_queue.begin()+found);
        return 1;
    }
    else
    {
        return 0;
    }
}

int CPU::change_current()
{
    int success = 0;
    if (ready_queue.size() == 0)
    {
        current = 1;
    }
    else
    {
        current = ready_queue[0];
        success = remove(current);
    }
    return success;
}
