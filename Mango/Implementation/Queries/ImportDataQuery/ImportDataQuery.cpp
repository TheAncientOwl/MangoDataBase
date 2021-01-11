#include "standard_library.hpp"
#include "ImportDataQuery.hpp"

#include "../../StringUtils/StringUtils.hpp"

#include "../../FileIO/CSV/CSV.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../../../ConsoleColorOutput/ConsoleColorOutput.hpp"

namespace Mango::Implementation::Queries
{
#pragma region MANGO_QUERY_API
	MANGO_QUERY_API bool ImportDataQuery::match(std::string_view sql) const
	{
		return sql.starts_with("IMPORT");
	}

	MANGO_QUERY_API void ImportDataQuery::parse(std::string_view sql)
	{
		m_TableName.clear();
		m_FileName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = StringUtils::splitAtChar(sql, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid import syntax");

		if (args[0] != "IMPORT")
			throw InvalidSyntaxException("Invalid import syntax");

		m_FileName = args[2];
		m_TableName = args[1];
	}

	MANGO_QUERY_API void ImportDataQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!dataBase.getTable(m_TableName))
			throw TableNotFoundException("Table does not exists.", std::move(m_TableName));

		if (!m_FileName.ends_with(".csv"))
			throw InvalidArgumentException("Import can be done only from .csv files");
	}

	MANGO_QUERY_API void ImportDataQuery::execute(ref<MangoDB> dataBase)
	{
		auto table = dataBase.getTable(m_TableName);
		
		std::ifstream csv;
		csv.open(m_FileName, std::ios::in);

		if (!csv.is_open())
			throw InvalidArgumentException("File does not exists or cannot be opened");

		std::cout << ccolor::dark_gray << ">> " << ccolor::dark_aqua << "Importing data" << ccolor::dark_gray << "...\n";

		std::string header;
		std::getline(csv, header);

		auto rowConfig = table->makeRowConfiguration();
		Row row(rowConfig->calculateTotalSize(), rowConfig);

		while (FileIO::CSV::read(csv, row))
		{
			table->insertRow(row);
		}

		csv.close();
	}
#pragma endregion

}