#include "standard_library.hpp"
#include "SaveDataQuery.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../../FileIO/CSV/CSV.hpp"

namespace Mango::Implementation::Queries
{
#pragma region MANGO_QUERY_API
	MANGO_QUERY_API bool SaveDataQuery::match(std::string_view sql) const
	{
		return sql.starts_with("SAVE");
	}

	MANGO_QUERY_API void SaveDataQuery::parse(std::string_view sql)
	{
		m_FileName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = StringUtils::splitAtChar(sql, ' ');

		if (args.size() != 2)
			throw InvalidSyntaxException("Invalid save syntax");

		if (args[0] != "SAVE")
			throw InvalidArgumentException({ "Unknwon sequence \"", args[0], "\"" });

		m_FileName = args[1];
	}

	MANGO_QUERY_API void SaveDataQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!(m_FileName.ends_with(".csv")))
			throw InvalidArgumentException("Data can be exported only in csv format");
	}

	MANGO_QUERY_API void SaveDataQuery::execute(ref<MangoDB> dataBase)
	{
		std::ofstream csv;
		csv.open(m_FileName, std::ios::out, std::ios::trunc);

		const auto& header = dataBase.m_LastColumns;
		for (size_t index = 0, size = header.size(); index < size; ++index)
			FileIO::CSV::write(csv, header[index], index < size - 1 ? ',' : '\n');

		for (const auto& row : dataBase.m_LastResult)
			FileIO::CSV::write(csv, row);

		csv.close();
	}
#pragma endregion

}