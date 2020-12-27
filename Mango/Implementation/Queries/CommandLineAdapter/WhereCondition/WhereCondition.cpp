#include "standard_library.hpp"
#include "WhereCondition.hpp"
#include "../../AbstractQuery/AbstractQuery.hpp"

using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries::CommandLineAdapter
{
	WhereClause WhereCondition::getClause() const
	{
		return m_Clause;
	}

	void WhereCondition::setClause(WhereClause clause)
	{
		m_Clause = clause;
	}

	bool WhereCondition::empty() const
	{
		return m_ColumnName.empty();
	}

	void WhereCondition::clear()
	{
		m_ColumnName.clear();
		m_Operation.clear();
		value.clear();
	}

	void WhereCondition::parseFrom(std::string_view condition)
	{
		auto args = AbstractQuery::splitAtChar(condition, ' ');

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

		m_ColumnName = args[0];
		m_Operation = args[1];
		value = value;
	}

	void WhereCondition::validate(std::string_view tableName, const_ref<MangoDB> dataBase)
	{
		if (!m_ColumnName.empty())
		{
			auto table = dataBase.getTable(tableName);

			ref<MangoDummyValues> dummy = MangoDummyValues::Instance();

			dummy.m_Index = static_cast<int>(table->getColumnIndex(m_ColumnName));
			if (dummy.m_Index == -1)
				throw InvalidArgumentException({ "Column \"", m_ColumnName, "\" does not exists" });

			switch (table->getColumn(dummy.m_Index).dataType())
			{
			case DataType::Value::INT:
			{
				m_Clause = RowFilters::Int::GetWhereClause(m_Operation);
				try
				{
					dummy.m_Int = std::stoi(value);
				}
				catch (...)
				{
					throw InvalidArgumentException({ "Cannot convert \"", value, "\" to int" });
				}
				break;
			}
			case DataType::Value::FLOAT:
			{
				m_Clause = RowFilters::Float::GetWhereClause(m_Operation);
				try
				{
					dummy.m_Float = std::stof(value);
				}
				catch (...)
				{
					throw InvalidArgumentException({ "Cannot convert \"", value, "\" to float" });
				}
				break;
			}
			case DataType::Value::STRING:
			{
				m_Clause = RowFilters::String::GetWhereClause(m_Operation);
				dummy.m_String = value;
				break;
			}
			}
		}
	}

}