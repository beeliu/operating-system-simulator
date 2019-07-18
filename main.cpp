/* 
    Belinda Liu
    CSCI34000 Operating Systems
    Section 1
    Home Project
*/

#include "os.h"
#include <iostream>
#include <ctype.h>
#include <cstdlib>
using namespace std;

int main()
{
    //asks for RAM memory (in bytes), up to 4000000000
    unsigned int ram_memory;
    bool first = 1;
    do
    {
        if (first == 1)
        {
            cout << "How much RAM memory is there on the simulated computer?"<< 
                     " Enter byte number between 0 and 4000000000" << endl;
            cin >> ram_memory;
            first = 0;
	}
        else
	{
	    cout << "Not a valid memory size. Please enter a number between"<<
    		    " 0 and 4000000000" << endl;
	    cin >> ram_memory; 
  	}
        
    }
    while (ram_memory < 0 || ram_memory > 4000000000);
   
    //Asks for size of a page/frame. Enumeration of pages starts from 0.
    unsigned int page_size;
    cout << "What is the size of a page/frame?" << endl;
    cin >> page_size;

    // Asks for number of hard disks.  Enumeration starts with 0.
    unsigned int disk_num;
    cout << "How many hard disks does the simulated computer have?" << endl;
    cin >> disk_num;

    // create OS object 
    OS os(ram_memory, page_size, disk_num);

    // keep track of pid numbers (do not reuse terminated process pids)
    int pid = 1;
    
    // Wait for input commands 
    string command_entered;
    while (1)  // continuous loop
    {
	getline(cin, command_entered);
	// parse command by whitespace and push into vector 
        std::vector<string> command;
	std::string temp_string;
	for (int i = 0; i < command_entered.length(); i++)
	{
	    if (command_entered[i] != ' ')
	    {
   	        temp_string = temp_string + command_entered[i];
 	    } 
	    else // blank
	    {
		command.push_back(temp_string);
		temp_string = "";
	    }
	    // push last onto command
	    if (i == command_entered.length()-1)
	    {
		command.push_back(temp_string);
	    }
	    
	}
	if (command.size() == 1)
	{
            if (command[0] == "A")
	    {
	        pid = pid + 1;
 	        os.command_a(pid);
	    }
            else if (command[0] == "Q")
	    {
	        os.command_q();
	    }
            else if (command[0] == "fork")
	    {
	        pid = pid + 1;
	        os.command_fork(pid);
	    }
	    else if (command[0] == "exit")
	    { 
	        os.command_exit();
	    }
 	    else if (command[0] == "wait")
	    {
	        os.command_wait();
	    }
	    else // not valid command 
	    {
		cout << "Please enter a valid command" << endl;
	    }
        }
	else if (command.size() == 2)
	{
            // D number
            if (command[0] == "D")
	    {
		bool digit = true;
		for (int j = 0; j < command[1].length(); j++)
	        {
		    if (!isdigit(command[1][j]))
		    {
			digit = false;
		    }
	        }
 	        if (digit)
	        {
		    int d_number = atoi(command[1].c_str());
		    os.command_d_finish(d_number); 
	        }
		else // not valid command
		{
		    cout << "Please enter valid command" << endl;
		}
 	    }
	    // m address
	    else if (command[0] == "m")
	    {
		// check if address is valid number
		bool digit = true;
		for (int i = 0; i < command[1].length(); i++)
		{
		    if (!isdigit(command[1][i]))
		    {
			digit = false;
		    }
		}
		if (digit)
		{
		    unsigned int address = strtoul(command[1].c_str(),NULL,0);
    		    os.command_m_address(address);
		}
		else // not valid address
		{
		    cout << "Please enter valid address" << endl;
		}
	    }
	    else if (command[0] == "S")
	    {
	        // S r
		if (command[1] == "r")
		{
		    std::string temp = os.command_s_r();
		    cout << temp << endl;
		}
 	        // S i
		else if (command[1] == "i")
		{
		    std::string temp = os.command_s_i();
		    cout << temp << endl;
		}
                // S m
		else if (command[1] == "m")
		{
                    std::string temp = os.command_s_m();
    		    cout << temp << endl;
		}
		else // not valid command
		{
		    cout << "Please enter valid command" << endl;
		}
   	    }
	    else // not valid command
	    {
		cout << "Please enter valid command" << endl;
	    }
        }
        else if (command.size() == 3)
        {
	    // d number file_name
	    if (command[0] == "d")
	    {
	 	bool digit = true;
		for (int j = 0; j < command[1].length(); j++)
		{
		    if (!isdigit(command[1][j]))
		    {
			digit = false;
		    }
		}
	 	if (digit)
	        {
		    int d_number = atoi (command[1].c_str());
		    if (d_number >= 0 && d_number < disk_num) // valid disk#
		    {
		        os.command_d_file(d_number, command[2]); 
	            }
		    else
		    {
			cout << "Please enter valid disk number" << endl;
		    }
  		}
	 	else // not valid command
		{
		    cout << "Please enter valid command" << endl;
		}
	    }
	    else // else not valid command
	    {
		cout << "Please enter valid command" << endl;
	    }
	}
	else // not a valid command, ask again and do nothing
	{
	    cout << "Please enter a valid command" << endl;
 	}
    }
}
