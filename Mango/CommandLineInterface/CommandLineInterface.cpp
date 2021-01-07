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

#define MAX(a, b) ((a) > (b) ? (a) : (b))

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

	int CommandLineInterface::digitsNo(int x) const
	{
		int n = 0;
		while (x > 0)
		{
			n++;
			x /= 10;
		}
		return n;
	}

	int CommandLineInterface::digitsNo(float x) const
	{
		while (x != (int)x)
			x *= 10;
		
		return digitsNo(int(x)) + 1;
	}

	void CommandLineInterface::displayResult(const_ref<std::vector<Implementation::Row>> rows, const_ref<std::vector<std::string>> columns) const
	{
		if (rows.empty())
			std::cout << "No data found\n";
		else
		{
			const int COLUMNS_NUMBER = static_cast<int>(columns.size());

			std::vector<int> maxColumnWidths;
			maxColumnWidths.reserve(COLUMNS_NUMBER);

			for (const auto& rowName : columns)
				maxColumnWidths.push_back(static_cast<int>(rowName.size()));

			/// calculate max widths.
			using Mango::Implementation::DataType;
			for (const auto& row : rows)
				for (int i = 0; i < COLUMNS_NUMBER; ++i)
				{
					int width = 0;
					switch (row.m_Config->dataTypeAt(i))
					{
						case DataType::Value::INT:    width = digitsNo(row.getInt(i));				     break;
						case DataType::Value::FLOAT:  width = digitsNo(row.getFloat(i));				 break;
						case DataType::Value::STRING: width = static_cast<int>(row.getString(i).size()); break;
					}

					maxColumnWidths[i] = MAX(width, maxColumnWidths[i]);
				}

			/// print column names.
			std::cout << ccolor::dark_gray << "| ";
			for (int i = 0; i < COLUMNS_NUMBER; ++i)
			{
				std::cout.width(maxColumnWidths[i]);
				std::cout << ccolor::dark_red << columns[i] << ccolor::dark_gray << " | ";
			}
			std::cout << '\n';

			/// print separator line.
			std::cout << ccolor::dark_gray << "|-";
			std::cout.fill('-');
			for (int i = 0; i < COLUMNS_NUMBER; ++i)
			{
				std::cout.width(maxColumnWidths[i]);
				std::cout << "-";
				std::cout.width(1);
				std::cout << (i < COLUMNS_NUMBER - 1 ? "-|-" : "-|");
			}
			std::cout.fill(' ');
			std::cout << '\n';

			/// print rows.
			for (const auto& row : rows)
			{
				std::cout << ccolor::dark_gray << "| ";
				for (int i = 0; i < COLUMNS_NUMBER; ++i)
				{
					std::cout.width(maxColumnWidths[i]);
					std::cout << ccolor::green;
					switch (row.m_Config->dataTypeAt(i))
					{
						case DataType::Value::INT:    std::cout << row.getInt(i);    break;
						case DataType::Value::FLOAT:  std::cout << row.getFloat(i);  break;
						case DataType::Value::STRING: std::cout << row.getString(i); break;
					}
					std::cout << ccolor::dark_gray << " | ";
				}
				std::cout << '\n';
			}
			std::cout.width(1);
		}
	}

	void CommandLineInterface::help() const
	{
		std::cout << ccolor::dark_gray << "\n|____________________________________[ ";
		std::cout << ccolor::purple << "Help";
		std::cout << ccolor::dark_gray << " ]_____________________________________|\n";

		CommandDescriptions::Exit::syntax(1);
		CommandDescriptions::Save::syntax(1);
		CommandDescriptions::Display::syntax(3);
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

	void CommandLineInterface::run()
	{
		std::cout << ccolor::dark_gray << "|_______________________________[ ";
		std::cout << ccolor::purple << "Mango Data Base";
		std::cout << ccolor::dark_gray << " ]_______________________________|\n";
		std::cout << ccolor::dark_aqua << ">> type '"
			<< ccolor::light_aqua << "help" << ccolor::dark_aqua << "'\n";

		MangoDB dataBase(m_DataBaseDirectoryPath);
		dataBase.loadTableConfigs();
		
		while (m_Running)
		{
			std::cout << ccolor::light_gray << "[" << ccolor::light_blue << "SQL" << ccolor::light_gray << "]: " << ccolor::lime;

			std::string sql;
			std::getline(std::cin, sql);
			
			bool success = true;
			std::pair<Implementation::Timer::Seconds, Implementation::Timer::Miliseconds> elapsedTime;
			try
			{
				Mango::Implementation::Timer timer;
				execute(sql, dataBase);
				elapsedTime = timer.elapsedTime();
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
				if (m_Select)
					displayResult(dataBase.lastResult(), dataBase.lastColumns());

				std::cout << ccolor::dark_gray << ">> ";
				std::cout << ccolor::dark_aqua << "Query took ";
				std::cout << ccolor::light_aqua << std::fixed << std::setprecision(2) << elapsedTime.first.count();
				std::cout << ccolor::dark_aqua << " s";
				std::cout << ccolor::dark_gray << " / ";
				std::cout << ccolor::light_aqua << std::fixed << std::setprecision(2) << elapsedTime.second.count();
				std::cout << ccolor::dark_aqua << " ms";
				std::cout << ccolor::dark_gray << ";\n";
			}

			std::cout << ccolor::dark_gray << "|_________________________________________________________________________________|\n";
		}
	}

	void CommandLineInterface::execute(ref<std::string> sql, ref<MangoDB> dataBase)
	{
		format(sql);

		m_Select = false;

		if (sql.starts_with("SELECT"))
			m_Select = true;
		else if (sql.starts_with("HELP"))
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
		: m_DataBaseDirectoryPath(dataBaseDirectoryPath)
	{
	}

	const std::array<std::unique_ptr<AbstractQuery>, 11> CommandLineInterface::s_Queries{
		std::make_unique<CreateTableQuery>(),
		std::make_unique<DropTableQuery>(),
		std::make_unique<TruncateTableQuery>(),
		std::make_unique<DisplayQuery>(),
		std::make_unique<InsertIntoQuery>(),
		std::make_unique<CommandLineAdapter::SelectQueryCLI>(),
		std::make_unique<CommandLineAdapter::DeleteQueryCLI>(),
		std::make_unique<CommandLineAdapter::UpdateQueryCLI>(),
		std::make_unique<SaveDataQuery>(),
		std::make_unique<ImportDataQuery>(),
		std::make_unique<ExecuteScriptQuery>()
	};
}