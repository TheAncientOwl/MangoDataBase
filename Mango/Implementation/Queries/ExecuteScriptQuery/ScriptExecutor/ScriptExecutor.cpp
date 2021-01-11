#include "standard_library.hpp"
#include "ScriptExecutor.hpp"

#include "../../../../CommandLineInterface/CommandLineInterface.hpp"

#include "../../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../../../../ConsoleColorOutput/ConsoleColorOutput.hpp"

namespace Mango::Implementation::Queries
{
	ScriptExecutor::ScriptExecutor(std::string fileName)
		: m_FileName(std::move(fileName))
	{
	}

	void ScriptExecutor::execute(ref<MangoDB> dataBase, ref<CommandLineInterface> cli) const
	{
		std::cout << ccolor::dark_gray << ">> " << ccolor::dark_aqua << "Executing script" << ccolor::dark_gray << "...\n";
		std::ifstream scriptFile;
		scriptFile.open(m_FileName, std::ios::in);

		if (!scriptFile.is_open())
			throw InvalidArgumentException("The file cannot be opened or does not exists");
		
		std::string sql;
		size_t line = 0;
		while (std::getline(scriptFile, sql))
		{
			line++;
			try
			{
				cli.execute(sql, dataBase);
			}
			catch (const MangoException& e)
			{
				std::cout << ccolor::dark_gray << "[";
				std::cout << ccolor::dark_red << "Error, line: " << line;
				std::cout << ccolor::dark_gray << "] ";
				std::cout << ccolor::light_red << e.what() << '\n';
			}
		}

		scriptFile.close();
	}
}