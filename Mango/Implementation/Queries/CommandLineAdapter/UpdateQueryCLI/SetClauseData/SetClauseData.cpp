#include "standard_library.hpp"
#include "SetClauseData.h"

#include "../../../AbstractQuery/AbstractQuery.hpp"

#define columnName first
#define columnValue second

#include "../../../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../../../../StringUtils/StringUtils.hpp"


namespace Mango::Implementation::Queries::CommandLineAdapter
{
	void SetClauseData::clear()
	{
		m_Data.clear();
		m_SetIndexes.clear();
	}

	void SetClauseData::parseFrom(std::string_view clause)
	{
		auto tuples = StringUtils::splitAtCharWithEscape(clause, ',');
		if (tuples.empty())
			throw InvalidSyntaxException("Empty set clause");

		for (const auto& tuple : tuples)
		{
			StringUtils::splitInCleanStringsAt(tuple, ' ', m_Args);

			if (m_Args.size() < 3)
				throw InvalidSyntaxException({ "Wrong syntax at \"", tuple, "\"" });

			if (m_Args[1] != "=")
				throw InvalidSyntaxException({ "Unknown sequence \"", m_Args[1], "\"" });

			StringUtils::removeEscapeChar(m_Args[2], ',');
			m_Data.emplace_back(std::move(m_Args[0]), std::move(m_Args[2]));
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