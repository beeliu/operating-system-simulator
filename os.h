/* 
    Belinda Liu
    Interface file for OS class
*/

#ifndef OS_H
#define OS_H

#include "cpu.h"
#include "pcb.h"
#include "disk.h"
#include "memory.h"
#include <map>
class OS
{
public:
    // constructor
    OS(unsigned int ram_mem,unsigned int page_sz, unsigned int disk_nm);
    // command functions
    void command_a(int p_id);
    void command_q();
    void command_fork(int p_id);
    void command_exit();
    void command_wait();
    void command_d_file(int number, std::string file_name);
    void command_d_finish(int number);
    std::string command_s_r();    
    std::string command_s_i();
    void command_m_address(unsigned int address);
    std::string command_s_m();
private:
    // ram memory
    unsigned int ram_memory;
    // page_size
    unsigned int page_size;
    // disk num
    std::vector<Disk> hard_disks;
    //list of all non terminated processes (map with pid/pcb pair)
    std::map<int,PCB> processes; 
    // list of all waiting processes(map)
    std::map<int,int> waiting_processes;
    // list of zombie processes(map)
    std::map<int,int> zombie_processes;
    // CPU 
    CPU cpu;
    //RAM
    Memory ram;

    // private helper function for command_exit()
	// returns a set of all pids that must be removed
    std::map<int,int> exit_children(int parent,std::map<int,PCB> processes); 
};


#endif /* OS_H */
