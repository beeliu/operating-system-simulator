/* 
   Belinda Liu
   Implementation file for Memory class
*/

#include "memory.h"

Memory::Memory(unsigned int sz, unsigned int pg_sz)
{
    total_size = sz;
    page_size = pg_sz;
    total_pages = (sz / pg_sz);
    // initialize ram object
    for (unsigned int i = 0; i < total_pages; i++)
    {
	MEM_OBJECT temp;
        temp.empty = true;
	temp.frame = i;
	ram.push_back(temp);
    }
    timestamp_counter = 0;
}

void Memory::request(int p_id, unsigned int address)
{
    // find page number of address in program
    unsigned int page_number = address/page_size;
    // if page number and pid are already in table
    // update timestamp and do nothing
    int frame_number;
    bool found = false;
    bool empty = false;
    int empty_index;
    int lru = ram[0].timestamp;
    int lru_index = 0;
    timestamp_counter = timestamp_counter + 1;
    for (int i = 0; i < ram.size(); i++)
    {
	if (ram[i].empty == false)
	{
            if (ram[i].timestamp < lru)
	    {
		lru = ram[i].timestamp;
		lru_index = i;
	    }
	    if (ram[i].pid == p_id && ram[i].page == page_number)
	    {
		frame_number = i;		
		found = true;	
	    }
	}
        if (ram[i].empty == true && empty == false)// mark index of first empty 
	{
	    empty_index = i;
	    empty = true;
	}
    }

    if (found)
    {
	// update timestamp for entry
	ram[frame_number].timestamp = timestamp_counter;	
    }
    else
    {
        // else if empty location on table, put there
        if (empty)
	{
            ram[empty_index].empty = false;
	    ram[empty_index].page = page_number;
	    ram[empty_index].pid = p_id;
            ram[empty_index].timestamp = timestamp_counter;
	}
        // else find LRU (biggest timestamp) in table and replace
	else
	{
            ram[lru_index].empty = false;
	    ram[lru_index].page = page_number;
	    ram[lru_index].pid = p_id;
	    ram[lru_index].timestamp = timestamp_counter;
	}
    }
}

void Memory::release(int p_id)
{
    // go through table and reset all table entries with pid p_id
    for (int i = 0; i < ram.size(); i++)
    {
	if (ram[i].pid == p_id)
	{
	    ram[i].empty = true; 
	}
    }
}

std::vector<MEM_OBJECT> Memory::return_table()
{
    // go through table and find non empty entries
    std::vector<MEM_OBJECT> temp;
    for (int i = 0; i < ram.size(); i++)
    {
        if (ram[i].empty == false)
	{
	    temp.push_back(ram[i]);
	}	
    }
    return temp;
}
