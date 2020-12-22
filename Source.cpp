#include "standard_library.hpp"

#include "Utilities/ConsoleTestLoop/ConsoleTestLoop.hpp"
#include "Mango/MangoAPI.hpp"

using namespace Mango;

int main()
{
	Table table;
	table.TEST();

	Utilities::ConsoleTestLoop consoleTestLoop;
	consoleTestLoop.run();

	//system("pause");
	return 0;
}