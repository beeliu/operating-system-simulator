/* 
   Belinda Liu
   Interface file for Memory class
   Simulates RAM
*/

#ifndef MEMORY_H
#define MEMORY_H

#include <vector>

struct MEM_OBJECT{
    bool empty;
    int frame;
    unsigned int page;
    int pid;
    int timestamp;
};

class Memory
{
public:
    Memory(unsigned int sz, unsigned int pg_sz); // constructor
    // fill table in order until full, then use LRU replacement
    void request(int p_id, unsigned int address);
    // release all memory associated with p_id
    void release(int p_id);
    std::vector<MEM_OBJECT> return_table();
    

private:
    unsigned int total_size;
    unsigned int page_size;
    unsigned int total_pages;
    std::vector<MEM_OBJECT> ram;
    int timestamp_counter; 
};

#endif /* MEMORY_H */
