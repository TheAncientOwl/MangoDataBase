#include "standard_library.hpp"

#include "Mango/MangoAPI.hpp"

/*
 * TO DO:
 *	-implement mango script executor
 *  -implement import data from csv files
*/

int main()
{
	Mango::CommandLineInterface cli("TestDataBase");
	cli.run();
	
	return 0;
}