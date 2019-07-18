/* 
    Belinda Liu
    Interface file for CPU class
*/

#ifndef CPU_H
#define CPU_H

#include <vector>

class CPU
{
public:
    // constructor
    CPU();
    // add to ready-queue, parameter is pid of process
    void add(int p_id);
    // get currently running process, returns pid of running process
    int get_running();
    // get all processes in queue, returns vector of process pids
    std::vector<int> get_all();
    // remove process from queue, takes in pid of process to be removed
        // and puts next process in CPU
    // must be IN queue, return 1 if removed, 0 if not
    int remove(int p_id);
    // change currently running process to next in queue, remove old
    int change_current();
private:
    // ready queue
    std::vector<int> ready_queue;
    // currently running process pid
    int current;
};

#endif /* CPU_H */
