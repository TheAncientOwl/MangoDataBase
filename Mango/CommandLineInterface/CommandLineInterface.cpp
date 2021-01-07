#include "standard_library.hpp"
#include "CommandLineInterface.hpp"

using namespace Mango::Implementation::Queries;

#include "../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../ConsoleColorOutput/ConsoleColorOutput.hpp"

#define SQL_BEGIN std::begin(sql)
#define SQL_END   std::end(sql)
#define CMD_END   std::begin(sql) + 6

namespace Mango
{
	void displayCommand(int number, std::string_view syntax)
	{
		std::cout << ccolor::dark_gray << "[";
		std::cout << ccolor::dark_red << number;
		std::cout << ccolor::dark_gray << "] ";
		std::cout << ccolor::green << syntax << '\n';
	}

	void CommandLineInterface::format(ref<std::string> sql) const
	{
		/// CREATE TABLE table_name (col1 dataType, col2 dataType, ...);
		/// DISPLAY */TABLE_NAME;
		/// DROP TABLE table_name;
		/// TRUNCATE TABLE table_name;

		/// SELECT */[col1, col2, ...] FROM table_name WHERE <col = value>;
		/// DELETE FROM table_name WHERE <col = val>;
		/// UPDATE table_name SET (col = val, ...) WHERE <col = val>;
		
		/// INSERT INTO table_name [col1, col2, ...] VALUES (val1, val2, ...);
		/// SAVE file_name.csv;
		/// IMPORT table_name file_name.csv;

		if (sql.size() > 5)
		{
			std::transform(SQL_BEGIN, CMD_END, SQL_BEGIN, ::toupper);

			if (sql.starts_with("SELECT") || sql.starts_with("DELETE"))
			{
				auto open = std::find(SQL_BEGIN, SQL_END, '<');
				auto closed = std::find(SQL_BEGIN, SQL_END, '>');

				if (open != SQL_END && closed != SQL_END && open < closed)
				{
					auto it = std::find(SQL_BEGIN, SQL_END, '<');

					std::string_view comparators = "=<>!";
					auto comparatorPos = std::find_first_of(std::next(it), SQL_END,
															std::cbegin(comparators), std::cend(comparators));

					std::transform(CMD_END, comparatorPos, CMD_END, ::toupper);
				}
				else std::transform(CMD_END, SQL_END, CMD_END, ::toupper);
			}
			else if (sql.starts_with("UPDATE"))
			{
				/// UPDATE table_name SET (col = val, ...) WHERE <col = val>;
				auto milestone = CMD_END;
				{
					auto open = std::find(CMD_END, SQL_END, '(');
					auto closed = std::find(CMD_END, SQL_END, ')');

					std::transform(CMD_END, open, CMD_END, ::toupper);

					if (open != SQL_END && closed != SQL_END && open < closed)
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
					auto open = std::find(milestone, SQL_END, '<');
					auto closed = std::find(milestone, SQL_END, '>');

					if (open != SQL_END && closed != SQL_END && open < closed)
					{
						auto it = std::find(milestone, SQL_END, '<');

						std::string_view comparators = "=<>!";
						auto comparatorPos = std::find_first_of(std::next(it), SQL_END,
															    std::cbegin(comparators), std::cend(comparators));

						std::transform(milestone, comparatorPos, CMD_END, ::toupper);
					}
				}
			}
			else if (sql.starts_with("INSERT"))
			{
				std::transform(CMD_END, std::find(CMD_END, SQL_END, '('), CMD_END, ::toupper);
			}
			else if (sql.starts_with("IMPORT"))
			{
				auto space = std::find(SQL_BEGIN, SQL_END, ' ');

				if (space != SQL_END)
				{
					space = std::find(std::next(space), SQL_END, ' ');
					std::transform(CMD_END, space, CMD_END, ::toupper);
				}
			}
			else if (!sql.starts_with("SAVE"))
			{
				std::transform(CMD_END, SQL_END, CMD_END, ::toupper);
			}
		}
		else std::transform(std::cbegin(sql), std::cend(sql), std::begin(sql), ::toupper);

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

			format(sql);

			bool select = false;

			if (sql.starts_with("SELECT"))
				select = true;
			else if (sql.starts_with("SYNTAX"))
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
				displayCommand(11, "EXIT");
				continue;
			}
			else if (sql.starts_with("EXIT"))
				return;

			bool success = true;
			try
			{	
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
					else
					{
						std::cout << ccolor::dark_gray << "| ";
						for (const auto& rowName : dataBase.lastColumns())
							std::cout << ccolor::dark_red << rowName << ccolor::dark_gray << " | ";
						std::cout << '\n';
						for (const auto& row : result)
							std::cout << row << '\n';
					}
				}
				std::cout << ccolor::dark_gray << "[";
				std::cout << ccolor::green << "Good";
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