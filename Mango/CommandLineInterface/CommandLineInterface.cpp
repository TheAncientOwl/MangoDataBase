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
				continue;
			}

			if (sql == "exit")
				return;

			bool success = true;
			try
			{
				if (sql.size() > 4)
				{
					auto wannaBeEndOfInsert = std::cbegin(sql) + 6;
					std::transform(std::cbegin(sql), wannaBeEndOfInsert, std::begin(sql), ::toupper);

					auto upperEnd = std::cend(sql);
					if (sql.starts_with("INSERT"))
						upperEnd = std::find(wannaBeEndOfInsert, std::cend(sql), '(');

					std::transform(wannaBeEndOfInsert, upperEnd, std::begin(sql) + 6, ::toupper);
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

	const std::array<std::unique_ptr<AbstractQuery>, 5> CommandLineInterface::s_Queries{
		std::make_unique<CreateTableQuery>(),
		std::make_unique<DropTableQuery>(),
		std::make_unique<TruncateTableQuery>(),
		std::make_unique<DisplayQuery>(),
		std::make_unique<InsertIntoQuery>()
	};
}