#include "standard_library.hpp"
#include "UpdateQuery.hpp"

namespace Mango::Queries
{
	using namespace Mango::Exceptions;

#pragma region MANGO_PRIVATE_API
	MANGO_QUERY_INTERFACE bool UpdateQuery::match(std::string_view sql) const
	{
		return sql.starts_with("UPDATE");
	}

	MANGO_QUERY_INTERFACE void UpdateQuery::parse(std::string_view sql)
	{
		m_TableName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = splitAtChar(sql, ' ');

		if (args.size() != 2)
			throw InvalidSyntaxException("Invalid update query syntax");

		if (args[0] != "UPDATE")
			throw InvalidArgumentException({ "Unknwon sequence \"", args[0], "\"" });

		m_TableName = args[1];
	}

	void UpdateQuery::validate(const_ref<MangoDB> dataBase)
	{
		auto table = dataBase.getTable(m_TableName);
		if (!table)
			throw TableNotFoundException("Table does not exist", std::move(m_TableName));
	}

	void UpdateQuery::execute(ref<MangoDB> dataBase)
	{
		auto table = dataBase.getTable(m_TableName);

		TableIterator tableIterator = table->makeIterator();

		if (dataBase.m_SetClause == &DefaultRowFilters::doNothing)
			assert(false && "Did you forget to set the set clause?");

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