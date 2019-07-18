/*
    Belinda Liu 
    Implementation file for Disk class
*/
#include "disk.h"

Disk::Disk(int num)
{
    disk_number = num;
    ready_queue; // size 0 initially
    current.pid = -1; // wait for command
    current.file_name = "";
}
void Disk::add(int p_id, std::string f_name)
{
    if (current.pid == -1)
    {
        current.pid = p_id;
        current.file_name = f_name;
    }
    else
    {
	QUEUE_OBJECT temp;
        temp.pid = p_id;
        temp.file_name = f_name;
        ready_queue.push_back(temp);
    }
}
int Disk::get_disk_number()
{
    return disk_number;
}

QUEUE_OBJECT Disk::get_running()
{
    return current;
}

std::vector<QUEUE_OBJECT> Disk::get_all()
{
    return ready_queue;
}

int Disk::remove(int p_id)
{
    int found = -1;
    for (int i = 0; i < ready_queue.size(); i ++)
    {
        if (p_id == ready_queue[i].pid)
	{
 	    found = i;
	}
    }
    if(found >=0)
    {
	ready_queue.erase(ready_queue.begin()+found);
	return 1;
    }
    else
    {
        return 0;
    }
}

int Disk::change_current()
{
    int success = 0;
    if (ready_queue.size() ==0)
    {
        current.pid = -1;
        current.file_name = "";
    }
    else
    {
        current = ready_queue[0];
	success = remove(current.pid);
    }
    return success;
}

std::string Disk::filename()
{
    return current.file_name;
}
