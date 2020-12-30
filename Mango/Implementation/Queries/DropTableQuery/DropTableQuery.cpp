#include "standard_library.hpp"
#include "DropTableQuery.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries
{
#pragma region MANGO_QUERY_API
	MANGO_QUERY_API bool DropTableQuery::match(std::string_view sql) const
	{
		return sql.starts_with("DROP");
	}

	MANGO_QUERY_API void DropTableQuery::parse(std::string_view sql)
	{
		m_TableName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = splitAtChar(sql, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid drop query syntax");

		if (args[0] != "DROP" || args[1] != "TABLE")
			throw InvalidArgumentException("Check missing \"DROP\" nor \"TABLE\" keywords");

		m_TableName = args[2];
	}

	MANGO_QUERY_API void DropTableQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!dataBase.getTable(m_TableName))
			throw TableNotFoundException("Table does not exist", std::move(m_TableName));
	}

	MANGO_QUERY_API void DropTableQuery::execute(ref<MangoDB> dataBase)
	{
		dataBase.removeTable(m_TableName);
	}
#pragma endregion

}