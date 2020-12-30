#include "standard_library.hpp"
#include "DeleteQueryCLI.hpp"

#include "../../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries::CommandLineAdapter
{
#pragma region MANGO_QUERY_API
	MANGO_QUERY_API void DeleteQueryCLI::parse(std::string_view sql)
	{
		m_TableName.clear();

		m_WhereCondition.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		Statement::iterator DEFAULT = std::cbegin(sql);
		Statement condition(DEFAULT, DEFAULT, "<", ">");

		for (auto it = std::cbegin(sql), end = std::cend(sql); it != end; ++it)
			switch (*it)
			{
				case '>': condition.closed = it; break;
				case '<': if (condition.open == DEFAULT) condition.open = it; break;
			}

		if (condition.open != DEFAULT || condition.closed != DEFAULT)
		{
			condition.checkValidOrder(DEFAULT);

			m_WhereCondition.parseFrom({ std::next(condition.open), condition.closed });

			auto args = StringUtils::splitAtChar({ std::cbegin(sql), condition.open }, ' ');

			if (args.size() != 4)
				throw InvalidSyntaxException("Invalid delete query syntax");

			if (args[0] != "DELETE" || args[1] != "FROM" || args[3] != "WHERE")
				throw InvalidArgumentException("Invalid delete query syntax");

			m_TableName = args[2];
		}
		else DeleteQuery::parse(sql);
	}

	MANGO_QUERY_API void DeleteQueryCLI::validate(const_ref<MangoDB> dataBase)
	{
		DeleteQuery::validate(dataBase);

		if (m_WhereCondition.empty())
			m_WhereCondition.setClause(&RowFilters::allwaysFalse);
		else
			m_WhereCondition.validate(m_TableName, dataBase);
	}

	MANGO_QUERY_API void DeleteQueryCLI::execute(ref<MangoDB> dataBase)
	{
		dataBase.setWhereClause(m_WhereCondition.getClause());

		DeleteQuery::execute(dataBase);
	}
#pragma endregion

	
}