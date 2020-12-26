#include "standard_library.hpp"
#include "WhereCondition.hpp"
#include "../../AbstractQuery/AbstractQuery.hpp"

namespace Mango::Queries::CommandLineAdapter
{
	using namespace Mango::Exceptions;

	bool WhereCondition::empty() const
	{
		return this->columnName.empty();
	}

	void WhereCondition::clear()
	{
		this->columnName.clear();
		this->operation.clear();
		this->value.clear();
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

		this->columnName = args[0];
		this->operation = args[1];
		this->value = value;
	}

	void WhereCondition::validate(std::string_view tableName, const_ref<MangoDB> dataBase)
	{
		if (!columnName.empty())
		{
			auto table = dataBase.getTable(tableName);

			ref<MangoDummyValues> dummy = MangoDummyValues::Instance();

			dummy.m_Index = static_cast<int>(table->getColumnIndex(columnName));
			if (dummy.m_Index == -1)
				throw InvalidArgumentException({ "Column \"", columnName, "\" does not exists" });

			switch (table->getColumn(dummy.m_Index).dataType())
			{
			case DataType::Value::INT:
			{
				clause = RowFilters::Int::GetWhereClause(operation);
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
				clause = RowFilters::Float::GetWhereClause(operation);
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
				clause = RowFilters::String::GetWhereClause(operation);
				dummy.m_String = value;
				break;
			}
			}
		}
	}

}