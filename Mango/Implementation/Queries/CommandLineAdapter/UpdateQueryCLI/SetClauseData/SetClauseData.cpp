#include "standard_library.hpp"
#include "SetClauseData.h"

#include "../../../AbstractQuery/AbstractQuery.hpp"

#define columnName first
#define columnValue second

#include "../../../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries::CommandLineAdapter
{
	void SetClauseData::clear()
	{
		m_Data.clear();
		m_SetIndexes.clear();
	}

	void SetClauseData::parseFrom(std::string_view clause)
	{
		auto tuples = AbstractQuery::splitAtChar(clause, ',');
		if (tuples.empty())
			throw InvalidSyntaxException("Empty set clause");

		ref<SetClauseData> setClauseData = SetClauseData::Instance();
		for (const auto& tuple : tuples)
		{
			auto args = AbstractQuery::splitAtChar(tuple, ' ');

			if (args.size() < 3)
				throw InvalidSyntaxException({ "Wrong syntax at \"", tuple, "\"" });

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

			if (args[1] != "=")
				throw InvalidSyntaxException({ "Unknown sequence \"", args[1], "\"" });

			setClauseData.m_Data.emplace_back(args[0], value);
		}
	}

	void SetClauseData::validate(const_ptr<Table> table) const
	{
		for (const auto& pair : m_Data)
			if (!table->getColumn(pair.columnName))
				throw InvalidArgumentException({ "Column \"", pair.columnName, "\" does not exists" });
	}

	void SetClauseData::extractRow(ptr<Table> table)
	{
		auto rowConfig = table->makeSharedRowConfiguration();
		m_Row = Row(rowConfig->calculateTotalSize(), rowConfig);

		for (const auto& pair : m_Data)
		{
			m_SetIndexes.push_back(static_cast<int>(table->getColumnIndex(pair.columnName)));

			m_Row.setValue(m_SetIndexes.back(), pair.columnValue);
		}
	}

	ref<SetClauseData> SetClauseData::Instance()
	{
		static SetClauseData dummy;
		return dummy;
	}

	void setClauseCLI(ref<Row> row)
	{
		ref<SetClauseData> setClauseData = SetClauseData::Instance();
		for (auto index : setClauseData.m_SetIndexes)
			row.copyValue(index, setClauseData.m_Row);
	}
}