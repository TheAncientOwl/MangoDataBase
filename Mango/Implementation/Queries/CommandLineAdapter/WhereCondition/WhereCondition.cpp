#include "standard_library.hpp"
#include "WhereCondition.hpp"
#include "../../AbstractQuery/AbstractQuery.hpp"

#include "../../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../../../StringUtils/StringUtils.hpp"

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
		m_Value.clear();
	}

	void WhereCondition::parseFrom(std::string_view condition)
	{
		StringUtils::splitInCleanStringsAt(condition, ' ', m_Args);

		if (m_Args.size() < 3)
			throw InvalidSyntaxException("Missing args at condition");
		
		if (m_Args.size() > 3)
			throw InvalidSyntaxException("Too many args at condition");

		m_ColumnName = std::move(m_Args[0]);
		m_Operation = std::move(m_Args[1]);
		m_Value = std::move(m_Args[2]);
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

			switch (table->getColumn(dummy.m_Index).getDataType())
			{
				case DataType::Value::INT:
				{
					m_Clause = RowFilters::Int::GetWhereClause(m_Operation);
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
					m_Clause = RowFilters::Float::GetWhereClause(m_Operation);
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
					m_Clause = RowFilters::String::GetWhereClause(m_Operation);
					dummy.m_String = m_Value;
					break;
				}
			}
		}
	}

}