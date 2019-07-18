/*
    Belinda Liu 
    Implementation file for OS class
*/

#include "os.h"
#include <stdlib.h>
#include <sstream>
OS::OS(unsigned int ram_mem, unsigned int page_sz, 
        unsigned int disk_nm): ram(ram_mem, page_sz) // initialize ram memory
{
    // initialize ram memory
    ram_memory = ram_mem;
    // initialize page size
    page_size = page_sz;
    // initialize disks
    for (int i = 0; i < disk_nm; i ++)
    {
	Disk temp_disk(i);
        hard_disks.push_back(temp_disk);
    }
    
    // create initial process with pid 1 and add to processes list
    PCB process1(1);
    processes.insert(std::pair<int,PCB>(1, process1));       
}

// command functions
void OS::command_a(int p_id)
{
    // create new PCB to process
    PCB temp(p_id);
    // add new process to list of all processes
    processes.insert(std::pair<int,PCB>(p_id, temp));
    // add new process to list of children of process1
    std::map<int,PCB>::iterator it = processes.find(1);
    (*it).second.add_child(p_id);
    //add to end of CPU ready queue
    cpu.add(p_id);   
}

void OS::command_q()
{
    cpu.add(cpu.get_running());
    cpu.change_current(); 
}

void OS::command_fork(int p_id)
{
    // create new PCB for process
    PCB temp(p_id);
    // add new process as child of currently running process
    int current = cpu.get_running();
    // find current in processes in OS and add temp as child
    std::map<int,PCB>::iterator pcb_current = processes.find(current);
    (*pcb_current).second.add_child(p_id);
    // change parent of new process to currently running process
    temp.set_parent(current);
    // add process to list of all processes
    processes.insert(std::pair<int,PCB>(p_id,temp));
    // add process to end of ready queue for CPU
    cpu.add(p_id);
}

void OS::command_exit()
{
    // get currently running process
    int current = cpu.get_running();

    // release memory
    ram.release(current);
    // find pcb in processes queue
    std::map<int,PCB>::iterator current_pcb = processes.find(current);
    // find children and for each child terminate process on queues and table
    // for EACH child:
    std::vector<int> children = (*current_pcb).second.get_children();
    // get map of all to delete
    std::map<int,int> all_exit = exit_children(current, processes); 
/* start of old code 
    for (int i = 0; i < children.size(); i++)
    {
	int child = children[i];
        // release memory for child
	ram.release(child);
	// remove children from CPU queue
	// if children not in CPU queue, look for them in disk queues
 	if (cpu.remove(child) == 0) 
	{
	    for (int j = 0; j < hard_disks.size(); j++)
	    {
		hard_disks[j].remove(child);	
	    }
	}
	// delete children from processes queue, waiting queue, zombie queue
        // find then erase
        std::map<int,PCB>::iterator it1;
        std::map<int,int>::iterator it2;
        std::map<int,int>::iterator it3;

        it1 = processes.find(child);
        it2 = waiting_processes.find(child);
	it3 = zombie_processes.find(child);
   
        if (it1 != processes.end())
	{
	    processes.erase(child);
	}
        if(it2 != waiting_processes.end())
	{
	    waiting_processes.erase(child);
	}
	if (it3 != zombie_processes.end())
	{
	    zombie_processes.erase(child);
	}
    }
End of old code */
// start of new code here
    //iterate through map all_exit
    for (std::map<int,int>::iterator it_exit = all_exit.begin(); 
		it_exit != all_exit.end(); it_exit++)
    {
	//get child number
        int child = (*it_exit).second;
	//release memory
 	ram.release(child);
	// remove children from CPU queue
	// if children not in queue, look in disk queues
	if (cpu.remove(child) == 0)
	{
	    for (int j = 0; j < hard_disks.size(); j++)
	    {
                if (hard_disks[j].get_running().pid == child)
		{
		    hard_disks[j].change_current();
		}
		hard_disks[j].remove(child);
	    }
	}
	// delete children from processes queue, waiting queue, zombie queue
	// find, then erase
	std::map<int,PCB>::iterator it1;
        std::map<int,int>::iterator it2;
        std::map<int,int>::iterator it3;

        it1 = processes.find(child);
        it2 = waiting_processes.find(child);
        it3 = zombie_processes.find(child);
   
        if (it1 != processes.end())
        {
            processes.erase(child);
        }
        if(it2 != waiting_processes.end())
        {
            waiting_processes.erase(child);
        }
        if (it3 != zombie_processes.end())
        {
            zombie_processes.erase(child);
        }

    }

// end of new code here
    // get parent 
    int parent = (*current_pcb).second.get_parent();
    // if parent waiting, put parent at end of r-queue
    std::map<int,int>::iterator it_parent;
    it_parent = waiting_processes.find(parent);
    if(it_parent != waiting_processes.end())
    {
        //delete parent from waiting processes
        waiting_processes.erase(parent);
        // add parent to ready queue
        cpu.add(parent);
	// delete pcb from all queues		
        cpu.change_current();
	std::map<int, PCB>::iterator it_current;
	it_current = processes.find(current);
	if (it_current != processes.end())
	{
	    processes.erase(current);
	}
    }
    // else move current process into zombie_processes and take off other queue
    else
    { 
        std::map<int,PCB>::iterator it_current;
        it_current = processes.find(current);
	cpu.change_current();
        if (it_current != processes.end())
        {
            processes.erase(current);
        }
	zombie_processes.insert(std::pair<int,int>(current, current));
    }
    
}

void OS::command_wait()
{
    // get current
    int current = cpu.get_running();
    // find in processes
    std::map<int,PCB>::iterator current_pcb = processes.find(current);
    // list of children
    std::vector<int> children = (*current_pcb).second.get_children();
    // if any child in zombie queue, delete child and continue CPU
    bool found = false;
    int count = 0;
    while (!found && count < children.size())
    {
        int child = children[count];
	count = count + 1;
        std::map<int,int>::iterator it_child;
        it_child = zombie_processes.find(child);
        if (it_child != zombie_processes.end())
	{
	    found = true;
            // delete child process from zombie processes
	    zombie_processes.erase(child);
	}
    }
    // else enter waiting queue, remove from cpu queue  
    if (!found)
    {
	cpu.change_current();
	waiting_processes.insert(std::pair<int,int>(current, current));
    }
}

void OS::command_d_file(int number, std::string file_name)
{
    if (cpu.get_running() > 1)
    {
        hard_disks[number].add(cpu.get_running(), file_name);
        cpu.change_current();
    }
}

void OS::command_d_finish(int number)
{
    // disk currently running DONE
    // get currently running on disk #
    int current = (hard_disks[number].get_running()).pid;
    // change current on disk #
    hard_disks[number].change_current();
    // return current to end of CPU ready-queue 
    cpu.add(current);
}

std::string OS::command_s_r()
{
    // temp string to return
    std::string temp = "CPU: "; 
    //currently running process
    std::stringstream ss;
    ss << cpu.get_running();
    temp = temp + ss.str() + "\n";
    // ready queue processes 
    temp = temp + "READY QUEUE: \n";
    std::vector<int> cpu_ready_queue = cpu.get_all();
    if (cpu_ready_queue.size() ==0)
    {
        temp = temp + "empty";
    }
    else
    {
        for (int i = 0; i < cpu_ready_queue.size(); i++)
        {
            std::stringstream ss2;
            ss2 << i;
            std::stringstream ss1;
            ss1 << cpu_ready_queue[i];
            temp = temp + ss2.str() + ": pid " +ss1.str() + "\n"; 
        }
    }
    return temp;
}

std::string OS::command_s_i()
{
    //go through each disk in hard_disks and print out currently running and 
    // waiting processes
    std::string temp = "";
    for (int i = 0; i < hard_disks.size(); i++)
    {
	QUEUE_OBJECT current = hard_disks[i].get_running();
	std::vector<QUEUE_OBJECT> ready_queue = hard_disks[i].get_all();
        std::stringstream ss1;
        ss1 << i;
	temp = temp + "DISK " + ss1.str() + ": ";
        // check if disk is idle
	if (current.pid > 1)
	{
	    std::stringstream ss;
	    ss << current.pid;
	    // currently running on disk 
	    temp = temp + "pid: " + ss.str() + " file: "
			+ current.file_name + "\n";
            // check if disk has ready queue items
	    temp = temp + "        QUEUE: \n";
	    if (ready_queue.size() > 0)
	    {
                // if ready queue not empty, print items
		for (int j = 0; j < ready_queue.size(); j++)
		{
		    std::stringstream ss3;
		    ss3 << ready_queue[j].pid;
		    std::stringstream ss4;
		    ss4 << j;
		    temp = temp + "        " + ss4.str()  +": pid: " + 
				ss3.str() + " file: " + 
				ready_queue[j].file_name + "\n";
		}
	    }
	    else // empty ready_queue
	    {
		temp = temp + "        empty \n";
	    }
 	}
	else // idle disk
	{
	    temp = temp + "idle \n";    
	}
    }
    return temp;
}

void OS::command_m_address(unsigned int address)
{
    // get currently running process
    int current = cpu.get_running();
    ram.request(current, address);  
}

std::string OS::command_s_m()
{
    std::vector<MEM_OBJECT> memory = ram.return_table();
    std::string temp = "Frame #       Page#      PID      timestamp\n";
    for (int i = 0; i < memory.size(); i++)
    {
        std::stringstream ss;
        ss << memory[i].frame << "              " << memory[i].page<< "         " <<
		memory[i].pid << "            " << memory[i].timestamp << "\n";
        temp = temp + ss.str();
    }
    return temp;
}

std::map<int,int> OS::exit_children(int parent,std::map<int,PCB> processes) 
{ 
    // map to store return value in
    std::map<int,int> temp;
    // keep track of when insertions are done
    bool all_found = false;
    bool first = true;
    while (!all_found) // go through processes
    {
	std::vector<int> temp_vector;
	if (first)
	{
	    first = false;
	    for (std::map<int,PCB>::iterator it = processes.begin(); 
		it != processes.end(); it++)
	    {
		// get parent of pcb
		int parent_it = (*it).second.get_parent();
		// get pid
		int pid_it = (*it).second.get_pid();
		// if parent is parent, add to temp then remove from processes
	        if (parent_it == parent)
		{
		    temp_vector.push_back(pid_it);
		    temp.insert(std::pair<int,int>(pid_it,pid_it));
		}
	    }
	    if (temp_vector.size() == 0)
	    {
		all_found = true;
	    }
	    else
	    {
	        // remove elements from processes
	        for (int i = 0; i < temp_vector.size(); i++)
	        {
 	            // find iterator to element and erase
		    std::map<int,PCB>::iterator it1 = 
			processes.find(temp_vector[i]);
		    processes.erase(it1);	
	        } 
	    }
	}// end of first if statement
	else//not first pass
	{
	    //reset temp_vector
	    temp_vector.clear();
	    //for each element of process, if it has parent in temp,
	    //push onto temp_vector and add to temp
            for (std::map<int,PCB>::iterator it2 = processes.begin();
                it2 != processes.end(); it2++)
	    {
	        // get parent of pcb
                int parent_it = (*it2).second.get_parent();
                // get pid
                int pid_it = (*it2).second.get_pid();
	        //iterate through temp
		for (std::map<int,int>::iterator it3 = temp.begin();
		    it3 != temp.end(); it3++)
		{
		    //get pids of temp
		    int pid_temp = (*it3).second;
		    if(pid_temp == parent_it)
		    {
			temp_vector.push_back(pid_it);
			temp.insert(std::pair<int,int>(pid_it,pid_it));
	  	    } 
		}
	    }
	    //erase temp_vector elements from processes
	    //if temp_vector == 0, all_found is true
	    if (temp_vector.size() == 0)
	    {
		all_found = true;
	    }
	    else
	    {
		// remove elements from processes
                for (int i = 0; i < temp_vector.size(); i++)
                {
                    // find iterator to element and erase
                    std::map<int,PCB>::iterator it4 =
                        processes.find(temp_vector[i]);
                    processes.erase(it4);
                }

	    }
	}// end of else statement
    }
    return temp;
}
