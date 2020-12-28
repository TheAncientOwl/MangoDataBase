#include "standard_library.hpp"

#include "Mango/CommandLineInterface/CommandLineInterface.hpp"

int main()
{
	/*using Mango::Implementation::Queries::AbstractQuery;
	
	while (true)
	{
		std::cout << ccolor::light_blue << ">> Input: " << ccolor::lime;
		std::string s;
		std::getline(std::cin, s);

		if (s == "exit")
			return 0;

		std::vector<std::string> args;
		AbstractQuery::splitWithStrings(s, ',', args);
		for (const auto& arg : args)
			std::cout << arg << '\n';
	}*/

	Mango::CommandLineInterface cli("TestDataBase");
	cli.run();


	return 0;
}