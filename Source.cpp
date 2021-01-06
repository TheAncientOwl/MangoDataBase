#include "standard_library.hpp"

#include "Mango/MangoAPI.hpp"

/*
 * TO DO:
 *	-implement mango script executor
*/

int main()
{
	Mango::CommandLineInterface cli("TestDataBase");
	cli.run();
	
	return 0;
}