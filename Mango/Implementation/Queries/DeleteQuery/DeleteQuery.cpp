#include "standard_library.hpp"
#include "DeleteQuery.hpp"

#include "../../BinaryUtils/BinaryUtils.hpp"
using namespace Mango::Implementation::BinaryUtils;

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../../StringUtils/StringUtils.hpp"


namespace Mango::Implementation::Queries
{
#pragma region MANGO_QUERY_API
	MANGO_QUERY_API bool DeleteQuery::match(std::string_view sql) const
	{
		return sql.starts_with("DELETE");
	}

	MANGO_QUERY_API void DeleteQuery::parse(std::string_view sql)
	{
		m_TableName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = StringUtils::splitAtChar(sql, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid delete query syntax");

		if (args[0] != "DELETE" || args[1] != "FROM")
			throw InvalidArgumentException("Invalid delete query syntax");

		m_TableName = args[2];
	}

	MANGO_QUERY_API void DeleteQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!dataBase.getTable(m_TableName))
			throw TableNotFoundException("Table does not exists", std::move(m_TableName));
	}

	MANGO_QUERY_API void DeleteQuery::execute(ref<MangoDB> dataBase)
	{
		MangoClauseGuard _(dataBase);
		auto table = dataBase.getTable(m_TableName);

		auto tableIterator = table->makeConstIterator();

		std::filesystem::path dummyFilePath = table->getDataFilePath();
		dummyFilePath.replace_filename("dummy");
		std::fstream dummyFile(dummyFilePath, std::ios::out | std::ios::trunc | std::ios::binary);

		while (tableIterator.advance())
		{
			auto& row = tableIterator.row();
			if (!dataBase.m_WhereClause(row))
				serializePOD(dummyFile, row.data(), row.size());
		}

		dummyFile.close();
		tableIterator.releaseFile();

		std::filesystem::remove(table->getDataFilePath());
		std::filesystem::rename(dummyFilePath, table->getDataFilePath());
	}
#pragma endregion

}