#include "standard_library.hpp"
#include "CommandLineInterface.hpp"

using namespace Mango::Implementation::Queries;

#include "../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../ConsoleColorOutput/ConsoleColorOutput.hpp"
#include "CommandDescriptions/CommandDescriptions.hpp"
#include "../Implementation/DataType/DataType.hpp"
#include "../Implementation/Timer/Timer.hpp"

#define SQL_BEGIN std::begin(sql)
#define SQL_END   std::end(sql)
#define CMD_END   std::begin(sql) + 6

#define MANGO_CLI_HEADER	ccolor::dark_gray << "|_______________________________[ "\
						 << ccolor::purple << "Mango Data Base"\
						 << ccolor::dark_gray << " ]_______________________________|\n"\
						 << ccolor::dark_aqua << ">> type '"\
						 << ccolor::light_aqua << "help" << ccolor::dark_aqua << "'"

#define MANGO_CLI_CMD_PREFIX ccolor::light_gray << "[" << ccolor::light_blue << "SQL" << ccolor::light_gray << "]: " << ccolor::lime

#define MANGO_CLI_ERROR_PREFIX	  ccolor::dark_gray << "["\
							   << ccolor::dark_red << "Error"\
	                           << ccolor::dark_gray << "] "\
							   << ccolor::light_red

#define MANGO_CLI_QUERY_TIME(elapsedTime)    ccolor::dark_gray << ">> "\
										  << ccolor::dark_aqua << "Query took "\
										  << ccolor::light_aqua << std::fixed << std::setprecision(4) << elapsedTime.count()\
										  << ccolor::dark_aqua << " s"\
										  << ccolor::dark_gray << ";\n"

#define MANGO_CLI_SEPARATOR_LINE ccolor::dark_gray << "|_________________________________________________________________________________|"

#define MANGO_CLI_HELP_HEADER    ccolor::dark_gray << "\n|____________________________________[ "\
							  << ccolor::purple << "Help"\
							  << ccolor::dark_gray << " ]_____________________________________|"

namespace Mango
{
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
		/// EXECUTE SCRIPT script.mango;
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

						std::transform(milestone, comparatorPos, milestone, ::toupper);
					}
				}
			}
			else if (sql.starts_with("INSERT"))
			{
				std::transform(CMD_END, std::find(CMD_END, SQL_END, '('), CMD_END, ::toupper);
			}
			else if (sql.starts_with("IMPORT") || sql.starts_with("EXEC"))
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

	void CommandLineInterface::help() const
	{
		std::cout << MANGO_CLI_HELP_HEADER << '\n';

		CommandDescriptions::Exit::syntax(1);
		CommandDescriptions::Display::syntax(2);
		CommandDescriptions::Desc::syntax(2);
		CommandDescriptions::Save::syntax(3);
		CommandDescriptions::Drop::syntax(4);
		CommandDescriptions::Truncate::syntax(5);
		CommandDescriptions::ExecuteScript::syntax(6);
		CommandDescriptions::Import::syntax(7);
		CommandDescriptions::Delete::syntax(8);
		CommandDescriptions::Create::syntax(9);
		CommandDescriptions::Update::syntax(10);
		CommandDescriptions::Select::syntax(11);
		CommandDescriptions::Insert::syntax(12);
	}

	ref<MangoDB> CommandLineInterface::dataBase()
	{
		return m_DataBase;
	}

	void CommandLineInterface::run()
	{
		std::cout << MANGO_CLI_HEADER << '\n';
		
		while (m_Running)
		{
			std::cout << MANGO_CLI_CMD_PREFIX;

			std::string sql;
			std::getline(std::cin, sql);
			
			try
			{
				Mango::Implementation::Timer timer;
				execute(sql, m_DataBase);

				std::cout << MANGO_CLI_QUERY_TIME(timer.elapsedTime());
			}
			catch (const MangoException& e)
			{
				std::cout << MANGO_CLI_ERROR_PREFIX << e.what() << '\n';
			}

			std::cout << MANGO_CLI_SEPARATOR_LINE << '\n';
		}
	}

	void CommandLineInterface::execute(ref<std::string> sql, ref<MangoDB> dataBase)
	{
		format(sql);

		if (sql.starts_with("HELP"))
		{
			help();
			return;
		}
		else if (sql.starts_with("EXIT"))
		{
			m_Running = false;
			return;
		}
		
		for (auto& query : s_Queries)
			if (query->match(sql))
			{
				query->parse(sql);
				query->validate(dataBase);
				query->execute(dataBase);
				
				return;
			}

		throw InvalidSyntaxException("Unknown command");
	}

	CommandLineInterface::CommandLineInterface(std::string_view dataBaseDirectoryPath)
		: m_DataBase(dataBaseDirectoryPath)
	{
		m_DataBase.loadTableConfigs();
	}

	const std::array<std::unique_ptr<AbstractQuery>, 12> CommandLineInterface::s_Queries{
		std::make_unique<CreateTableQuery>(),
		std::make_unique<DropTableQuery>(),
		std::make_unique<TruncateTableQuery>(),
		std::make_unique<DescQuery>(),
		std::make_unique<InsertIntoQuery>(),
		std::make_unique<CommandLineAdapter::SelectQueryCLI>(),
		std::make_unique<CommandLineAdapter::DeleteQueryCLI>(),
		std::make_unique<CommandLineAdapter::UpdateQueryCLI>(),
		std::make_unique<SaveDataQuery>(),
		std::make_unique<ImportDataQuery>(),
		std::make_unique<ExecuteScriptQuery>(),
		std::make_unique<DisplayQuery>()
	};
}