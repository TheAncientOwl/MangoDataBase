#include "standard_library.hpp"
#include "CommandLineInterface.hpp"

using namespace Mango::Implementation::Queries;

#include "../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../ConsoleColorOutput/ConsoleColorOutput.hpp"

namespace Mango
{
	void displayCommand(int number, std::string_view syntax)
	{
		std::cout << ccolor::dark_gray << "[";
		std::cout << ccolor::dark_red << number;
		std::cout << ccolor::dark_gray << "] ";
		std::cout << ccolor::green << syntax << '\n';
	}

	void CommandLineInterface::run() const
	{
		MangoDB dataBase(m_DataBaseDirectoryPath);
		dataBase.loadTableConfigs();
		
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
				displayCommand(8, MANGO_UPDATE_CLI_SYNTAX);
				displayCommand(9, MANGO_SAVE_DATA_SYNTAX);
				displayCommand(10, MANGO_IMPORT_DATA_SYNTAX);
				displayCommand(11, "exit");
				continue;
			}

			if (sql == "exit")
				return;

			bool success = true;
			try
			{
				/// INSERT INTO table_name [col1, col2, ...] VALUES (val1, val2, ...);
				/// DELETE FROM table_name WHERE <col = val>;
				/// SELECT */[col1, col2, ...] FROM (table_name) WHERE <col = value>;
				/// UPDATE table_name SET (col = val, ...) WHERE <col = val>;
				
				if (sql.size() > 5)
				{
					std::transform(std::begin(sql), std::begin(sql) + 6, std::begin(sql), ::toupper);

					if (!sql.starts_with("SAVE"))
					{
						if (sql.starts_with("INSERT"))
						{
							auto firstParanthesis = std::find(std::cbegin(sql), std::cend(sql), '(');
							std::transform(std::cbegin(sql), firstParanthesis, std::begin(sql), ::toupper);
						}
						else if (sql.starts_with("IMPORT"))
						{
							auto space = std::find(std::begin(sql), std::end(sql), ' ');
							if (space != std::end(sql))
							{
								space = std::find(std::next(space), std::end(sql), ' ');
								std::transform(std::begin(sql) + 6, space, std::begin(sql) + 6, ::toupper);
							}
						}
						else if (sql.starts_with("SELECT") || sql.starts_with("DELETE"))
						{
							auto open = std::find(std::cbegin(sql), std::cend(sql), '<');
							auto closed = std::find(std::cbegin(sql), std::cend(sql), '>');

							if (open != std::cend(sql) && closed != std::cend(sql) && open < closed)
							{
								auto it = std::find(std::cbegin(sql), std::cend(sql), '<');

								std::string_view comparators = "=<>!";
								auto comparatorPos = std::find_first_of(std::next(it), std::cend(sql),
									std::cbegin(comparators), std::cend(comparators));

								std::transform(std::cbegin(sql), comparatorPos, std::begin(sql), ::toupper);
							}
							else std::transform(std::begin(sql), std::end(sql), std::begin(sql), ::toupper);
						}
						else if (sql.starts_with("UPDATE"))
						{
							auto milestone = std::begin(sql);

							{
								auto open = std::find(std::begin(sql), std::end(sql), '(');
								auto closed = std::find(std::begin(sql), std::end(sql), ')');

								std::transform(std::begin(sql), open, std::begin(sql), ::toupper);

								if (open != std::end(sql) && closed != std::end(sql) && open < closed)
								{
									bool upper = true;
									for (std::string::iterator prev = open, it = std::next(open); it != closed; ++it, ++prev)
									{
										if (*it == '"')
										{
											if (*prev == '/')
											{
												continue;
											}
											upper = !upper;
											continue;
										}

										if (upper)
											*it = ::toupper(*it);
									}

									milestone = closed;
								}
							}

							{
								auto open = std::find(milestone, std::end(sql), '<');
								auto closed = std::find(milestone, std::end(sql), '>');

								if (open != std::end(sql) && closed != std::end(sql) && open < closed)
								{
									auto it = std::find(milestone, std::end(sql), '<');

									std::string_view comparators = "=<>!";
									auto comparatorPos = std::find_first_of(std::next(it), std::end(sql),
										std::cbegin(comparators), std::cend(comparators));

									std::transform(milestone, comparatorPos, milestone, ::toupper);
								}
							}

						}
						else std::transform(std::begin(sql), std::end(sql), std::begin(sql), ::toupper);
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
					auto& result = dataBase.lastResult();
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

	CommandLineInterface::CommandLineInterface(std::string_view dataBaseDirectoryPath)
		: m_DataBaseDirectoryPath(dataBaseDirectoryPath)
	{
	}

	const std::array<std::unique_ptr<AbstractQuery>, 10> CommandLineInterface::s_Queries{
		std::make_unique<CreateTableQuery>(),
		std::make_unique<DropTableQuery>(),
		std::make_unique<TruncateTableQuery>(),
		std::make_unique<DisplayQuery>(),
		std::make_unique<InsertIntoQuery>(),
		std::make_unique<CommandLineAdapter::SelectQueryCLI>(),
		std::make_unique<CommandLineAdapter::DeleteQueryCLI>(),
		std::make_unique<CommandLineAdapter::UpdateQueryCLI>(),
		std::make_unique<SaveDataQuery>(),
		std::make_unique<ImportDataQuery>()
	};
}