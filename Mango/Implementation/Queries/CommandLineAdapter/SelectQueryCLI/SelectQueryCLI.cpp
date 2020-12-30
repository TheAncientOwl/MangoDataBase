#include "standard_library.hpp"
#include "SelectQueryCLI.hpp"

#include "../../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries::CommandLineAdapter
{
#pragma region MANGO_API
	MANGO_API void SelectQueryCLI::checkStatementsOrder(Statement columns, Statement table, Statement condition,
														        Statement::iterator defaultIt) const
	{
		columns.checkValidOrder(defaultIt);

		table.checkValidOrder(defaultIt);

		condition.checkValidOrder(defaultIt);

		if (table.open < columns.closed)
			throw InvalidSyntaxException({ "Syntax error, found '", table.openChar, "' before '", columns.closedChar, "'" });

		if (condition.open < table.closed)
			throw InvalidSyntaxException({ "Syntax error, found '", condition.openChar, "' before '", table.closedChar, "'" });
	}

	MANGO_API void SelectQueryCLI::checkResidualParts(Statement columns, Statement table, Statement condition, 
														      std::string_view sql) const
	{
		{
			std::string_view part(std::cbegin(sql), columns.open);
			auto args = splitAtChar(part, ' ');
			if (args.size() > 1 || args.front() != "SELECT")
				throw InvalidArgumentException({ "Unknown sequence \"", part, "\"" });
		}

		{
			std::string_view part(std::next(columns.closed), table.open);
			auto args = splitAtChar(part, ' ');
			if (args.empty())
				throw InvalidSyntaxException("Missing \"FROM\" keyword");
			if (args.size() != 1 || args.front() != "FROM")
				throw InvalidArgumentException({ "Unknown sequence \"", part, "\"" });
		}

		{
			std::string_view part(std::next(table.closed), condition.open);
			auto args = splitAtChar(part, ' ');
			if (args.empty())
				throw InvalidSyntaxException("Missing \"WHERE\" keyword");
			if (args.size() != 1 || args.front() != "WHERE")
				throw InvalidArgumentException({ "Unknown sequence \"", part, "\"" });
		}

		{
			std::string_view part(std::next(condition.closed), std::cend(sql));
			part.remove_suffix(1);
			if (!part.empty())
				throw InvalidSyntaxException({ "Unknown sequence \"", part, "\"" });
		}
	}
#pragma endregion

#pragma region MANGO_QUERY_API
	void SelectQueryCLI::parse(std::string_view sql)
	{
		m_TableName.clear();
		m_ColumnNames.clear();

		m_WhereCondition.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		Statement::iterator DEFAULT = std::cbegin(sql);
		Statement::iterator all = DEFAULT;
		Statement columns(DEFAULT, DEFAULT, "[", "]"), table(DEFAULT, DEFAULT, "(", ")"), condition(DEFAULT, DEFAULT, "<", ">");

		for (auto it = std::cbegin(sql), end = std::cend(sql); it != end; ++it)
			switch (*it)
			{
				case ']': columns.closed = it; break;
				case ')': table.closed = it; break;
				case '>': condition.closed = it; break;
				case '*': all = it; break;
				case '[': if (columns.open == DEFAULT) columns.open = it; break;
				case '(': if (table.open == DEFAULT) table.open = it; break;
				case '<': if (condition.open == DEFAULT) condition.open = it; break;
			}

		if (all != DEFAULT)
		{
			if (columns.open != DEFAULT || columns.closed != DEFAULT)
				throw InvalidSyntaxException({ "Choose between '*' and '", columns.openChar,
											  "column1, column2, ...", columns.closedChar, "'" });
			columns.open = all;
			columns.closed = all;
			columns.openChar = columns.closedChar = "*";
		}

		if (all == DEFAULT)
			SelectQuery::parseColumnNames({ std::next(columns.open), columns.closed });
		SelectQuery::parseTableName({ std::next(table.open), table.closed });

		if (condition.open != DEFAULT || condition.closed != DEFAULT)
		{
			SelectQueryCLI::checkStatementsOrder(columns, table, condition, DEFAULT);
			SelectQueryCLI::checkResidualParts(columns, table, condition, sql);
			
			m_WhereCondition.parseFrom({ std::next(condition.open), condition.closed });
		}
		else
		{
			SelectQuery::checkStatementsOrder(columns, table, DEFAULT);
			SelectQuery::checkResidualParts(columns, table, sql);
		}
	}

	MANGO_QUERY_API void SelectQueryCLI::validate(const_ref<MangoDB> dataBase)
	{
		SelectQuery::validate(dataBase);

		if (m_WhereCondition.empty())
			m_WhereCondition.setClause(&RowFilters::allwaysTrue);
		else
			m_WhereCondition.validate(m_TableName, dataBase);
	}

	MANGO_QUERY_API void SelectQueryCLI::execute(ref<MangoDB> dataBase)
	{
		dataBase.setWhereClause(m_WhereCondition.getClause());

		SelectQuery::execute(dataBase);
	}
#pragma endregion

}