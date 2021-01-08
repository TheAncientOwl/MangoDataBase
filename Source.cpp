#include "standard_library.hpp"

#include "Mango/MangoAPI.hpp"

#include "Mango/ConsoleColorOutput/ConsoleColorOutput.hpp"

int main(int argc, char** argv)
{
	Mango::CommandLineInterface cli(PROJECT_DATA_BASE);
	
	if (argc == 1)
	{
		cli.run();
	}
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			std::string sql;
			sql.reserve(17 + strlen(argv[1]));
			sql.append("EXECUTE SCRIPT ").append(argv[i]).append(";");

			try
			{
				cli.execute(sql, cli.dataBase());
			}
			catch (const Mango::Exceptions::MangoException& e)
			{
				std::cout << ccolor::dark_gray << "[";
				std::cout << ccolor::dark_red << "Error";
				std::cout << ccolor::dark_gray << "] ";
				std::cout << ccolor::light_red << e.what() << '\n';
			}
		}

		std::cout << ccolor::dark_gray << ">> " << ccolor::green << "Scripts executed" << ccolor::dark_gray << ".\n";
		
		cli.run();
	}

	std::cout << ccolor::reset;
}