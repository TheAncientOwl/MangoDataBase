#include "standard_library.hpp"
#include "CommandLineInterface.hpp"

namespace Mango
{
	void displayCommand(int number, std::string_view syntax)
	{
		std::cout << ccolor::dark_gray << "[";
		std::cout << ccolor::dark_red << number;
		std::cout << ccolor::dark_gray << "] ";
		std::cout << ccolor::green << syntax << '\n';
	}

	void CommandLineInterface::run()
	{
		MangoDB dataBase("DataBaseFromCommandLine");
		dataBase.loadTables();
		
		while (true)
		{
			std::string sql;
			std::cout << ccolor::light_gray << "[" << ccolor::light_blue << "SQL" << ccolor::light_gray << "]: " << ccolor::lime;
			std::getline(std::cin, sql);

			bool select = false;
			if (sql.starts_with("select") || sql.starts_with("SELECT"))
				select = true;

			if (sql == "syntax")
			{
				displayCommand(1, MANGO_CREATE_TABLE_SYNTAX);
				displayCommand(2, MANGO_INSERT_INTO_SYNTAX);
				displayCommand(3, MANGO_TRUNCATE_TABLE_SYNTAX);
				displayCommand(4, MANGO_DROP_TABLE_SYNTAX);
				displayCommand(5, MANGO_DISPLAY_SYNTAX);
				displayCommand(6, MANGO_SELECT_CLI_SYNTAX);
				displayCommand(7, MANGO_DELETE_CLI_SYNTAX);
				continue;
			}

			if (sql == "exit")
				return;

			bool success = true;
			try
			{
				if (sql.size() > 5)
				{
					if (sql.starts_with("INSERT") || sql.starts_with("insert"))
					{
						auto firstParanthesis = std::find(std::cbegin(sql), std::cend(sql), '(');

						std::transform(std::cbegin(sql), firstParanthesis, std::begin(sql), ::toupper);
					}
					else
					{
						std::string_view str(sql);
						Statement::iterator DEFAULT = std::cbegin(str);
						Statement condition(std::find(std::cbegin(str), std::cend(str), '<'), std::find(std::cbegin(str), std::cend(str), '>'), "<", ">");
						if (condition.open != DEFAULT || condition.closed != DEFAULT)
						{
							condition.checkValidOrder(DEFAULT);
							std::string_view comparators = "=<>!";

							auto it = std::find(std::cbegin(sql), std::cend(sql), '<');
							if (it != std::cend(sql))
							{
								auto comparatorPos = std::find_first_of(std::next(it), std::cend(sql),
									std::cbegin(comparators), std::cend(comparators));
								std::transform(std::cbegin(sql), comparatorPos, std::begin(sql), ::toupper);
							}
							else std::transform(std::begin(sql), std::end(sql), std::begin(sql), ::toupper);
							
						}
						
					}
				}
				else std::transform(std::begin(sql), std::end(sql), std::begin(sql), ::toupper);

				bool found = false;
				for (auto& query : s_Queries)
					if (query->match(sql))
					{
						query->parse(sql);
						query->validate(dataBase);
						query->execute(dataBase);

						found = true;
						break;
					}

				if (!found)
					throw InvalidSyntaxException("Unknown command");
			}
			catch (const MangoException& e)
			{
				success = false;
				std::cout << ccolor::dark_gray << "[";
				std::cout << ccolor::dark_red << "Error";
				std::cout << ccolor::dark_gray << "] ";
				std::cout << ccolor::light_red << e.what() << '\n';
			}

			if (success)
			{
				if (select)
				{
					auto result = dataBase.extractLastResult();
					if (result.empty())
						std::cout << "No data found\n";
					else for (const auto& row : result)
						std::cout << row << '\n';
					
				}
				std::cout << ccolor::dark_gray << "[";
				std::cout << ccolor::green << "Nice";
				std::cout << ccolor::dark_gray << "] ";
				std::cout << ccolor::green << "Command executed successfully!\n";

			}
		}

	}

	const std::array<std::unique_ptr<AbstractQuery>, 7> CommandLineInterface::s_Queries{
		std::make_unique<CreateTableQuery>(),
		std::make_unique<DropTableQuery>(),
		std::make_unique<TruncateTableQuery>(),
		std::make_unique<DisplayQuery>(),
		std::make_unique<InsertIntoQuery>(),
		std::make_unique<CommandLineAdapter::SelectQueryCLI>(),
		std::make_unique<CommandLineAdapter::DeleteQueryCLI>()
	};
}