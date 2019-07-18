/* 
    Belinda Liu
    Interface file for PCB class (process control block)
*/

#ifndef PCB_H  //include guard
#define PCB_H

#include <vector>

class PCB
{
public:
    //constructor that takes in p_id as parameter
    PCB(int p_id);
    // returns pid of process
    int get_pid();
    // set parent process 
    void set_parent(int par);
    // add child process
    void add_child(int ch);
    //remove child process
    void remove_child(int ch); 
    // returns parent process
    int get_parent();
    // returns vector of children
    std::vector<int> get_children();
private:
    // pid of process
    //ready queue (one for each disk)
    int pid;
    // state of process:
	// only keep track if 1: waiting or 2: terminating/exit
 	// 0: if ready/running
    // keep track of parent and child processes (ids)
    int parent;
    std::vector<int> children;
};

#endif /* PCB_H */
