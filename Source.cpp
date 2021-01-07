#include "standard_library.hpp"

#include "Mango/MangoAPI.hpp"

int main()
{
	Mango::CommandLineInterface cli("TestDataBase");
	cli.run();
	
	return 0;
}