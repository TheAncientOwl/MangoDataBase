#include "standard_library.hpp"
#include "TruncateTableQuery.hpp"

namespace Mango::Queries
{
	using namespace Mango::Exceptions;

	bool QUERY_API TruncateTableQuery::match(std::string_view sql) const
	{
		return sql.starts_with("TRUNCATE");
	}

	void QUERY_API TruncateTableQuery::parse(std::string_view sql)
	{
		m_TableName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = splitAtChar(sql, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid truncate query syntax");

		if (args[0] != "TRUNCATE" || args[1] != "TABLE")
			throw InvalidArgumentException("Check missing \"TRUNCATE\" nor \"TABLE\" keywords");

		m_TableName = args[2];
	}

	void QUERY_API TruncateTableQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!dataBase.getTable(m_TableName))
			throw TableNotFoundException("Table does not exist", std::move(m_TableName));
	}

	void QUERY_API TruncateTableQuery::execute(ref<MangoDB> dataBase)
	{
		dataBase.getTable(m_TableName)->clearDataFile();
	}
}