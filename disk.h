/*
    Belinda Liu 
    Interface file for Disk class
*/

#ifndef DISK_H
#define DISK_H

#include <vector>
#include <string>

struct QUEUE_OBJECT{
    int pid;
    std::string file_name;
};
class Disk
{
public:
    //constructor (takes in disk number)
    Disk(int num);
    // add to ready-queue, or if empty ready queue make current 
    void add(int p_id, std::string f_name);
    // get disk number (returns int)
    int get_disk_number();
    // get currently running process on disk (returns pid)
    QUEUE_OBJECT get_running();
    // get all QUEUE_OBJECTs in ready queue
    std::vector<QUEUE_OBJECT> get_all();
    // remove process from ready queue (parameter is pid of process)
       // and lets next process use disk
    int remove(int p_id);
    int change_current();
    // return filename being accessed by currently running process
    std::string filename();   
private:
    // disk number
    int disk_number;
    // ready queue of pids
    std::vector<QUEUE_OBJECT> ready_queue;
    // currently running process pid
    QUEUE_OBJECT current;
        
};

#endif /* DISK_H */
