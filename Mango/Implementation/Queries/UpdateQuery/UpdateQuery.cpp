#include "standard_library.hpp"
#include "UpdateQuery.hpp"

#include "../../Row/RowFilters.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries
{
#pragma region MANGO_QUERY_API
	MANGO_QUERY_API bool UpdateQuery::match(std::string_view sql) const
	{
		return sql.starts_with("UPDATE");
	}

	MANGO_QUERY_API void UpdateQuery::parse(std::string_view sql)
	{
		m_TableName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = StringUtils::splitAtChar(sql, ' ');

		if (args.size() != 2)
			throw InvalidSyntaxException("Invalid update query syntax");

		if (args[0] != "UPDATE")
			throw InvalidArgumentException({ "Unknwon sequence \"", args[0], "\"" });

		m_TableName = args[1];
	}

	MANGO_QUERY_API void UpdateQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!dataBase.getTable(m_TableName))
			throw TableNotFoundException("Table does not exist", std::move(m_TableName));
	}

	MANGO_QUERY_API void UpdateQuery::execute(ref<MangoDB> dataBase)
	{
		MangoClauseGuard _(dataBase);

		auto table = dataBase.getTable(m_TableName);

		TableIterator tableIterator = table->makeIterator();

		assert(dataBase.m_SetClause != &RowFilters::doNothing && "Did you forget to set the set clause?");

		while (tableIterator.advance())
		{
			Row& row = tableIterator.row();
			if (dataBase.m_WhereClause(row))
			{
				dataBase.m_SetClause(row);
				tableIterator.overwrite(row);
			}
		}

		tableIterator.releaseFile();
	}
#pragma endregion

}