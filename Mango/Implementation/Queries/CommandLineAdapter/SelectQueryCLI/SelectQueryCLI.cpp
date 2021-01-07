#include "standard_library.hpp"
#include "SelectQueryCLI.hpp"

#include "../../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries::CommandLineAdapter
{
#pragma region MANGO_API
	MANGO_API void SelectQueryCLI::parseTableName(std::string_view tablePart)
	{
		auto args = StringUtils::splitAtChar(tablePart, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid select syntax");

		if (args[0] != "FROM" || args[2] != "WHERE")
			throw InvalidSyntaxException({ "Unknown sequence \"", tablePart, "\"" });

		m_TableName = args[1];
	}

	void SelectQueryCLI::checkStatementsOrder(Statement columns, Statement condition, Statement::iterator defaultIt) const
	{
		columns.checkValidOrder(defaultIt);

		condition.checkValidOrder(defaultIt);

		if (condition.open < columns.closed)
			throw InvalidSyntaxException({ "Syntax error, found '", condition.openChar, "' before '", columns.closedChar, "'" });
	}

	MANGO_API void SelectQueryCLI::checkResidualParts(Statement columns, Statement condition, std::string_view sql) const
	{
		{
			std::string_view part(std::cbegin(sql), columns.open);
			auto args = StringUtils::splitAtChar(part, ' ');
			if (args.size() > 1 || args.front() != "SELECT")
				throw InvalidArgumentException({ "Unknown sequence \"", part, "\"" });
		}

		{
			std::string_view part(std::next(condition.closed), std::cend(sql));

			while (!part.empty() && part.front() == ' ')
				part.remove_prefix(1);

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
		sql.remove_suffix(1);

		Statement::iterator DEFAULT = std::cbegin(sql);
		Statement::iterator all = DEFAULT;
		Statement columns(DEFAULT, DEFAULT, "[", "]"), condition(DEFAULT, DEFAULT, "<", ">");

		for (auto it = std::cbegin(sql), end = std::cend(sql); it != end; ++it)
			switch (*it)
			{
				case ']': columns.closed = it; break;
				case '>': condition.closed = it; break;
				case '*': all = it; break;
				case '[': if (columns.open == DEFAULT) columns.open = it; break;
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

		columns.checkValidOrder(DEFAULT);
		if (all == DEFAULT)
			SelectQuery::parseColumnNames({ std::next(columns.open), columns.closed });

		if (condition.open != DEFAULT || condition.closed != DEFAULT)
		{
			SelectQueryCLI::checkStatementsOrder(columns, condition, DEFAULT);
			SelectQueryCLI::checkResidualParts(columns, condition, sql);

			SelectQueryCLI::parseTableName({ std::next(columns.closed), condition.open });
			
			m_WhereCondition.parseFrom({ std::next(condition.open), condition.closed });
		}
		else
		{
			SelectQuery::parseTableName({ std::next(columns.closed), std::cend(sql) });
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