#include "standard_library.hpp"
#include "CommandDescriptions.hpp"

#include "../../ConsoleColorOutput/ConsoleColorOutput.hpp"

#define BRACKETS_NUMBER ccolor::dark_gray << "[" << ccolor::dark_red << number << ccolor::dark_gray << "]"

#define COMMAND_COLOR ccolor::dark_aqua
#define TABLE_COLOR ccolor::purple
#define COLUMN_COLOR ccolor::green
#define VALUE_COLOR ccolor::lime
#define FILE_COLOR ccolor::lime

namespace Mango::CommandDescriptions
{
	namespace Create
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';
			
			std::cout << COMMAND_COLOR << "CREATE TABLE ";
			std::cout << TABLE_COLOR << "table_name ";
			std::cout << ccolor::dark_gray << "(";
			std::cout << COLUMN_COLOR << "column1 " << VALUE_COLOR << "dataType" << ccolor::dark_gray << ", ";
			std::cout << COLUMN_COLOR << "column2 " << VALUE_COLOR << "dataType" << ccolor::dark_gray << ", ";
			std::cout << COLUMN_COLOR << "...";
			std::cout << ccolor::dark_gray << ");\n";
		}

		void desc()
		{
		}
	}

	namespace Delete
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';

			std::cout << COMMAND_COLOR << "DELETE FROM ";
			std::cout << TABLE_COLOR << "table_name ";

			std::cout << COMMAND_COLOR << "WHERE ";
			std::cout << ccolor::dark_gray << "<";
			std::cout << COLUMN_COLOR << "column";
			std::cout << ccolor::dark_gray << " = ";
			std::cout << VALUE_COLOR << "value";
			std::cout << ccolor::dark_gray << ">;\n";
		}

		void desc()
		{
		}
	}

	namespace Display
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';

			std::cout << COMMAND_COLOR << "DISPLAY ";
			std::cout << TABLE_COLOR << "*" << ccolor::dark_gray << "/" << TABLE_COLOR << "table_name";
			std::cout << ccolor::dark_gray << ";\n";
		}

		void desc()
		{

		}
	}

	namespace Drop
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';

			std::cout << COMMAND_COLOR << "DROP TABLE ";
			std::cout << TABLE_COLOR << "table_name";
			std::cout << ccolor::dark_gray << ";\n";
		}

		void desc()
		{

		}
	}

	namespace Insert
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';

			std::cout << COMMAND_COLOR << "INSERT INTO ";
			std::cout << TABLE_COLOR << "table_name ";

			std::cout << ccolor::dark_gray << "[";
			std::cout << COLUMN_COLOR << "column1" << ccolor::dark_gray << ", ";
			std::cout << COLUMN_COLOR << "column2" << ccolor::dark_gray << ", ";
			std::cout << COLUMN_COLOR << "...";
			std::cout << ccolor::dark_gray << "]";

			std::cout << COMMAND_COLOR << " VALUES ";

			std::cout << ccolor::dark_gray << "(";
			std::cout << VALUE_COLOR << "value1" << ccolor::dark_gray << ", ";
			std::cout << VALUE_COLOR << "value2" << ccolor::dark_gray << ", ";
			std::cout << VALUE_COLOR << "...";
			std::cout << ccolor::dark_gray << ");\n";
		}

		void desc()
		{

		}
	}

	namespace Truncate
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';

			std::cout << COMMAND_COLOR << "TRUNCATE TABLE ";
			std::cout << TABLE_COLOR << "table_name";
			std::cout << ccolor::dark_gray << ";\n";
		}

		void desc()
		{

		}
	}

	namespace Save
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';

			std::cout << COMMAND_COLOR << "SAVE ";
			std::cout << FILE_COLOR << "file_name.csv";
			std::cout << ccolor::dark_gray << ";\n";
		}

		void desc()
		{

		}
	}

	namespace Import
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';

			std::cout << COMMAND_COLOR << "IMPORT ";
			std::cout << TABLE_COLOR << "table_name ";
			std::cout << FILE_COLOR << "file_name.csv";
			std::cout << ccolor::dark_gray << ";\n";
		}

		void desc()
		{

		}
	}

	namespace Select
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';

			std::cout << COMMAND_COLOR << "SELECT ";
			std::cout << COMMAND_COLOR << "*";
			std::cout << ccolor::dark_gray << "/[";
			std::cout << COMMAND_COLOR << "column1" << ccolor::dark_gray << ", ";
			std::cout << COMMAND_COLOR << "column2" << ccolor::dark_gray << ", ";
			std::cout << COMMAND_COLOR << "...";
			std::cout << ccolor::dark_gray << "] ";
			std::cout << COMMAND_COLOR << "FROM ";
			std::cout << TABLE_COLOR << "table_name ";

			std::cout << COMMAND_COLOR << "WHERE ";
			std::cout << ccolor::dark_gray << "<";
			std::cout << COLUMN_COLOR << "column";
			std::cout << ccolor::dark_gray << " = ";
			std::cout << VALUE_COLOR << "value";
			std::cout << ccolor::dark_gray << ">;\n";
		}

		void desc()
		{

		}
	}

	namespace Update
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';

			std::cout << COMMAND_COLOR << "UPDATE ";
			std::cout << TABLE_COLOR << "table_name ";
			std::cout << COMMAND_COLOR << "SET ";
			std::cout << ccolor::dark_gray << "(";
			std::cout << COLUMN_COLOR << "column";
			std::cout << ccolor::dark_gray << " = ";
			std::cout << VALUE_COLOR << "value";
			std::cout << ccolor::dark_gray << ", ";
			std::cout << COLUMN_COLOR << "...";
			std::cout << ccolor::dark_gray << ") ";

			std::cout << COMMAND_COLOR << "WHERE ";
			std::cout << ccolor::dark_gray << "<";
			std::cout << COLUMN_COLOR << "column";
			std::cout << ccolor::dark_gray << " = ";
			std::cout << VALUE_COLOR << "value";
			std::cout << ccolor::dark_gray << ">;\n";
		}

		void desc()
		{

		}
	}

	namespace Exit
	{
		void syntax(int number)
		{
			std::cout << BRACKETS_NUMBER << ' ';
			
			std::cout << COMMAND_COLOR << "EXIT";
			std::cout << ccolor::dark_gray << ";\n";
		}
	}
}