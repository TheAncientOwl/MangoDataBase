#include "standard_library.hpp"
#include "ConsoleTestLoop.hpp"

namespace Utilities
{
	void ConsoleTestLoop::run()
	{
		Mango::MangoDB dataBase("TestDataBase");
		dataBase.loadTables();
		std::cout << dataBase << '\n';

		Mango::QueryExecutor queryExecutor;

		while (true)
		{
			std::string sql;
			std::cout << ccolor::light_gray << "[" << ccolor::light_blue << "SQL" << ccolor::light_gray << "]: " << ccolor::lime;
			std::getline(std::cin, sql);

			bool select = false;
			if (sql.starts_with("select") || sql.starts_with("SELECT"))
				select = true;

			if (sql == "exit")
				return;
			
			bool success = true;
			try
			{
				queryExecutor.execute(std::move(sql), dataBase);
			}
			catch (const Mango::Exceptions::MangoException& e)
			{
				success = false;
				std::cout << ccolor::dark_gray << "[";
				std::cout << ccolor::dark_red;

				using namespace Mango::Exceptions;
				switch (e.type())
				{
				case MangoExceptionType::INVALID_SYNTAX: std::cout << "Invalid Syntax"; break;
				case MangoExceptionType::INVALID_ARGUMENT: std::cout << "Invalid Argument"; break;
				case MangoExceptionType::NaN: std::cout << "NaN"; break;
				case MangoExceptionType::TABLE_ALREADY_EXISTS: std::cout << "Table already exists"; break;
				case MangoExceptionType::TABLE_NOT_FOUND: std::cout << "Table not found"; break;
				case MangoExceptionType::FEATURE_NOT_IMPLEMENTED: std::cout << "Feature not implemented"; break;
				}

				std::cout << ccolor::dark_gray << "] ";
				std::cout << ccolor::light_red << e.what() << '\n';
			}

			if (success)
			{
				if (select)
				{
					auto result = dataBase.extractLastResult();
					for (const auto& row : result)
					{
						std::cout << row << '\n';
					}
				}
				std::cout << ccolor::dark_gray << "[";
				std::cout << ccolor::green << "Nice";
				std::cout << ccolor::dark_gray << "] ";
				std::cout << ccolor::green << "Good command!\n";
				
			}
		}
	}
}