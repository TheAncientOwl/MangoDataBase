#include "standard_library.hpp"

#include "Mango/CommandLineInterface/CommandLineInterface.hpp"

#include "Angajat.hpp"

int main()
{
	Mango::CommandLineInterface cli("TestDataBase");
	cli.run();


	return 0;
}