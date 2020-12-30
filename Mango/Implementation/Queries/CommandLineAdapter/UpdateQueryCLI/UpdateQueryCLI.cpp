#include "standard_library.hpp"
#include "UpdateQueryCLI.hpp"

#include "SetClauseData/SetClauseData.h"

#include "../../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries::CommandLineAdapter
{
#pragma region MANGO_API
	MANGO_API void UpdateQueryCLI::checkStatementsOrder(Statement setClause, Statement condition, 
															    Statement::iterator defaultIt) const
	{
		setClause.checkValidOrder(defaultIt);

		condition.checkValidOrder(defaultIt);

		if (condition.open < setClause.closed)
			throw InvalidSyntaxException({ "Syntax error, found '", condition.openChar, "' before '", setClause.closedChar, "'" });
	}

	MANGO_API void UpdateQueryCLI::checkResidualParts(Statement setClause, Statement condition,
							                Statement::iterator defaultIt, Statement::iterator stringEnd) const
	{
		{
			std::string_view part(std::next(setClause.closed), condition.open);
			auto args = splitAtChar(part, ' ');

			if (args.empty())
				throw InvalidSyntaxException({ "Missing \"WHERE\" keyword" });

			if (args.size() != 1)
				throw InvalidSyntaxException({ "Unknown sequence \"", part, "\"" });

			if (args.front() != "WHERE")
				throw InvalidSyntaxException({ "Missing \"WHERE\" keyword" });
		}

		{
			std::string_view part(std::next(condition.closed), stringEnd);
			if (part.size() > 1)
				throw InvalidSyntaxException({ "Unknown sequence \"", part, "\"" });
		}
	}

	MANGO_API void UpdateQueryCLI::parseTableName(std::string_view firstPart)
	{
		auto args = splitAtChar(firstPart, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid update query syntax");

		if (args[0] != "UPDATE" || args[2] != "SET")
			throw InvalidArgumentException("Check missing \"UPDATE\" nor \"SET\" keywords");

		m_TableName = args[1];
	}
#pragma endregion


#pragma region MANGO_QUERY_API
	MANGO_QUERY_API void UpdateQueryCLI::parse(std::string_view sql)
	{
		m_TableName.clear();
		
		m_WhereCondition.clear();
		SetClauseData::Instance().clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		Statement::iterator DEFAULT = std::cbegin(sql);
		Statement setClause(DEFAULT, DEFAULT, "(", ")"), condition(DEFAULT, DEFAULT, "<", ">");

		for (auto it = std::cbegin(sql), end = std::cend(sql); it != end; ++it)
			switch (*it)
			{
				case '>': condition.closed = it; break;
				case ')': setClause.closed = it; break;
				case '<': if (condition.open == DEFAULT) condition.open = it; break;
				case '(': if (setClause.open == DEFAULT) setClause.open = it; break;
			}

		checkStatementsOrder(setClause, condition, DEFAULT);

		checkResidualParts(setClause, condition, DEFAULT, std::cend(sql));
		
		parseTableName({ std::cbegin(sql), setClause.open });

		SetClauseData::Instance().parseFrom({ std::next(setClause.open), setClause.closed });

		m_WhereCondition.parseFrom({ std::next(condition.open), condition.closed });
	}

	MANGO_QUERY_API void UpdateQueryCLI::validate(const_ref<MangoDB> dataBase)
	{
		auto table = dataBase.getTable(m_TableName);
		if (!table)
			throw TableNotFoundException("Table does not exists", std::move(m_TableName));

		if (m_WhereCondition.empty())
			m_WhereCondition.setClause(&RowFilters::allwaysTrue);
		else
			m_WhereCondition.validate(m_TableName, dataBase);

		SetClauseData::Instance().validate(table);
	}

	MANGO_QUERY_API void UpdateQueryCLI::execute(ref<MangoDB> dataBase)
	{
		SetClauseData::Instance().extractRow(dataBase.getTable(m_TableName));
		dataBase.setSetClause(&setClauseCLI);

		dataBase.setWhereClause(m_WhereCondition.getClause());

		UpdateQuery::execute(dataBase);
	}
#pragma endregion

}