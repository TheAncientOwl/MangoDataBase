#include "standard_library.hpp"
#include "DeleteQueryCLI.hpp"

namespace Mango::Queries::CommandLineAdapter
{
	using namespace Mango::Exceptions;

#pragma region MANGO_PRIVATE_API
	MANGO_PRIVATE_API void DeleteQueryCLI::parseCondition(std::string_view condition)
	{
		std::cout << condition << '\n';
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
			if (value.front() != '"' || value.back() != '"')
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
	MANGO_QUERY_INTERFACE void DeleteQueryCLI::parse(std::string_view sql)
	{
		m_TableName.clear();

		m_WhereColumnName.clear();
		m_Operation.clear();
		m_Value.clear();

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

			parseCondition({ std::next(condition.open), condition.closed});

			auto args = splitAtChar({ std::cbegin(sql), condition.open }, ' ');

			if (args.size() != 4)
				throw InvalidSyntaxException("Invalid delete query syntax");

			if (args[0] != "DELETE" || args[1] != "FROM" || args[3] != "WHERE")
				throw InvalidArgumentException("Invalid delete query syntax");

			m_TableName = args[2];
		}
		else DeleteQuery::parse(sql);
	}

	MANGO_QUERY_INTERFACE void DeleteQueryCLI::validate(const_ref<MangoDB> dataBase)
	{
		DeleteQuery::validate(dataBase);

		if (m_WhereColumnName.empty())
			m_WhereClause = &RowFilters::allwaysFalse;
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

	MANGO_QUERY_INTERFACE void DeleteQueryCLI::execute(ref<MangoDB> dataBase)
	{
		dataBase.setWhereClause(m_WhereClause);

		DeleteQuery::execute(dataBase);
	}
#pragma endregion

	
}