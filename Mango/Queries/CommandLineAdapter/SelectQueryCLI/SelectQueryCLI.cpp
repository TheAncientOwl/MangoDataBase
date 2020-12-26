#include "standard_library.hpp"
#include "SelectQueryCLI.hpp"

namespace Mango::Queries::CommandLineAdapter
{
	using namespace Mango::Exceptions;

#pragma region MANGO_PRIVATE_API
	MANGO_PRIVATE_API void SelectQueryCLI::checkStatementsOrder(Statement columns, Statement table, Statement condition,
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

	MANGO_PRIVATE_API void SelectQueryCLI::checkResidualParts(Statement columns, Statement table, Statement condition, 
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

	MANGO_PRIVATE_API void SelectQueryCLI::parseCondition(std::string_view condition)
	{
		auto args = splitAtChar(condition, ' ');

		if (args.size() < 3)
			throw InvalidSyntaxException("Missing args at condition");

		std::string_view value;

		if (args.size() > 3)
			value = std::string_view(args[2].data(), std::next(&args.back().back()));
		else
			value = args[2];

		if (value.front() == '"' || value.back() == '"')
		{
			if(value.front() != '"' || value.back() != '"')
					throw InvalidSyntaxException("Missing '\"'");
			value.remove_prefix(1);
			value.remove_suffix(1);
		}

		m_WhereColumnName = args[0];
		m_Operation = args[1];
		m_Value = value;
	}
#pragma endregion

#pragma region MANGO_QUERY_INTERFACE
	void SelectQueryCLI::parse(std::string_view sql)
	{
		m_TableName.clear();
		m_ColumnNames.clear();

		m_WhereColumnName.clear();
		m_Operation.clear();
		m_Value.clear();

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
			SelectQuery::parseColumns({ std::next(columns.open), columns.closed });
		SelectQuery::parseTableName({ std::next(table.open), table.closed });

		if (condition.open != DEFAULT || condition.closed != DEFAULT)
		{
			SelectQueryCLI::checkStatementsOrder(columns, table, condition, DEFAULT);
			SelectQueryCLI::checkResidualParts(columns, table, condition, sql);
			
			SelectQueryCLI::parseCondition({ std::next(condition.open), condition.closed });
		}
		else
		{
			SelectQuery::checkStatementsOrder(columns, table, DEFAULT);
			SelectQuery::checkResidualParts(columns, table, sql);
		}
	}

	MANGO_QUERY_INTERFACE void SelectQueryCLI::validate(const_ref<MangoDB> dataBase)
	{
		SelectQuery::validate(dataBase);

		if (m_WhereColumnName.empty())
			m_WhereClause = &RowFilters::allwaysTrue;
		else
		{
			auto table = dataBase.getTable(m_TableName);

			ref<MangoDummyValues> dummy = MangoDummyValues::Instance();

			dummy.m_Index = static_cast<int>(table->getColumnIndex(m_WhereColumnName));
			if (dummy.m_Index == -1)
				throw InvalidArgumentException({ "Column \"", m_WhereColumnName, "\" does not exists" });
		
			switch (table->getColumn(dummy.m_Index).dataType())
			{
				case DataType::Value::INT:
				{
					m_WhereClause = RowFilters::Int::GetWhereClause(m_Operation);
					try
					{
						dummy.m_Int = std::stoi(m_Value);
					}
					catch (...)
					{
						throw InvalidArgumentException({ "Cannot convert \"", m_Value, "\" to int" });
					}
					break;
				}
				case DataType::Value::FLOAT:
				{
					m_WhereClause = RowFilters::Float::GetWhereClause(m_Operation);
					try
					{
						dummy.m_Float = std::stof(m_Value);
					}
					catch (...)
					{
						throw InvalidArgumentException({ "Cannot convert \"", m_Value, "\" to float" });
					}
					break;
				}
				case DataType::Value::STRING:
				{
					m_WhereClause = RowFilters::String::GetWhereClause(m_Operation);
					dummy.m_String = m_Value;
					break;
				}
			}
		}

	}

	MANGO_QUERY_INTERFACE void SelectQueryCLI::execute(ref<MangoDB> dataBase)
	{
		dataBase.setWhereClause(m_WhereClause);

		SelectQuery::execute(dataBase);
	}
#pragma endregion

}