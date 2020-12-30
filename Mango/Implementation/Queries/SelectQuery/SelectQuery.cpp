#include "standard_library.hpp"
#include "SelectQuery.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../../StringUtils/StringUtils.hpp"

namespace Mango::Implementation::Queries
{
#pragma region MANGO_API
	MANGO_API void SelectQuery::checkStatementsOrder(Statement columns, Statement table, Statement::iterator defaultIt) const
	{
		columns.checkValidOrder(defaultIt);

		table.checkValidOrder(defaultIt);

		if (table.open < columns.closed)
			throw InvalidSyntaxException({ "Syntax error, found '", table.openChar, "' before '", columns.closedChar, "'" });
	}

	MANGO_API void SelectQuery::checkResidualParts(Statement columns, Statement table, std::string_view sql) const
	{
		{
			std::string_view part(std::cbegin(sql), columns.open);
			auto args = StringUtils::splitAtChar(part, ' ');
			if (args.size() > 1 || args.front() != "SELECT")
				throw InvalidArgumentException({ "Unknown sequence \"", part, "\"" });
		}

		{
			std::string_view part(std::next(columns.closed), table.open);
			auto args = StringUtils::splitAtChar(part, ' ');
			if (args.empty())
				throw InvalidSyntaxException("Missing \"FROM\" keyword");
			if (args.size() != 1 || args.front() != "FROM")
				throw InvalidArgumentException({ "Unknown sequence \"", part, "\"" });
		}

		{
			std::string_view part(std::next(table.closed), std::cend(sql));
			while (part.starts_with(' '))
				part.remove_prefix(1);
			part.remove_suffix(1);
			if (!part.empty())
				throw InvalidSyntaxException({ "Unknown sequence \"", part, "\"" });
		}

	}

	MANGO_API void SelectQuery::parseColumnNames(std::string_view columnsPart)
	{
		auto args = StringUtils::splitAtChar(columnsPart, ',');
		if (args.empty())
			throw InvalidSyntaxException("Since [] are used, column names must be specified");

		for (const auto& arg : args)
			m_ColumnNames.emplace_back(StringUtils::trimWhiteSpaces(arg));
	}

	MANGO_API void SelectQuery::parseTableName(std::string_view tablePart)
	{
		auto args = StringUtils::splitAtChar(tablePart, ' ');

		if (args.empty())
			throw InvalidSyntaxException("No table specified");

		if (args.size() != 1)
			throw InvalidSyntaxException({ "Unknown sequence \"", tablePart, "\"" });

		m_TableName = args.front();
	}

	MANGO_API void SelectQuery::selectAll(ptr<Table> table, ref<MangoDB> dataBase) const
	{
		auto rowConfig = table->makeSharedRowConfiguration();
		size_t rowSize = rowConfig->calculateTotalSize();

		ConstTableIterator tableIterator = table->makeConstIterator();

		auto& result = dataBase.m_LastResult;
		result.emplace_back(rowSize, rowConfig);

		while (tableIterator.advanceInPlace(result.back()))
			if (dataBase.m_WhereClause(result.back()))
				result.emplace_back(rowSize, rowConfig);
		result.pop_back();
	}

	MANGO_API void SelectQuery::selectSome(ptr<Table> table, ref<MangoDB> dataBase) const
	{
		std::vector<int> selectedColumnIndexes;
		auto rowConfig = std::make_shared<RowConfiguration>();

		for (const auto& columnName : m_ColumnNames)
		{
			selectedColumnIndexes.push_back(static_cast<int>(table->getColumnIndex(columnName)));
			const_ref<Column> column = table->getColumn(selectedColumnIndexes.back());

			rowConfig->pushBack(column.size(), column.dataType());
		}

		size_t rowSize = rowConfig->calculateTotalSize();

		auto& result = dataBase.m_LastResult;
		ConstTableIterator tableIterator = table->makeConstIterator();
		bool selected = true;
		while (tableIterator.advance())
		{
			if (selected)
				result.emplace_back(rowSize, rowConfig);

			selected = dataBase.m_WhereClause(tableIterator.row());

			if (selected)
			{
				auto& lastRow = result.back();

				for (int currentColumn = 0; currentColumn < m_ColumnNames.size(); ++currentColumn)
					lastRow.setDataAt(currentColumn, tableIterator.row().dataAt(selectedColumnIndexes[currentColumn]), 
									  rowConfig->sizeAt(currentColumn));

			}
		}
		if (!selected)
			result.pop_back();
	}
#pragma endregion

#pragma region MANGO_QUERY_API
	MANGO_QUERY_API bool SelectQuery::match(std::string_view sql) const
	{
		return sql.starts_with("SELECT");
	}

	MANGO_QUERY_API void SelectQuery::parse(std::string_view sql)
	{
		m_TableName.clear();
		m_ColumnNames.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		Statement::iterator DEFAULT = std::cbegin(sql);
		Statement::iterator all = DEFAULT;
		Statement columns(DEFAULT, DEFAULT, "[", "]"), table(DEFAULT, DEFAULT, "(", ")");

		for (auto it = std::cbegin(sql), end = std::cend(sql); it != end; ++it)
			switch (*it)
			{
				case ']': columns.closed = it; break;
				case ')': table.closed = it; break;
				case '*': all = it; break;
				case '[': if (columns.open == DEFAULT) columns.open = it; break;
				case '(': if (table.open == DEFAULT) table.open = it; break;
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

		checkStatementsOrder(columns, table, DEFAULT);

		checkResidualParts(columns, table, sql);

		if (all == DEFAULT)
			parseColumnNames({ std::next(columns.open), columns.closed });

		parseTableName({ std::next(table.open), table.closed });
	}

	MANGO_QUERY_API void SelectQuery::validate(const_ref<MangoDB> dataBase)
	{
		auto table = dataBase.getTable(m_TableName);
		if (!table)
			throw TableNotFoundException("Table does not exist", std::move(m_TableName));

		if (!m_ColumnNames.empty())
		{
			for (const auto& columnName : m_ColumnNames)
				if (!table->getColumn(columnName))
					throw InvalidArgumentException({ "Column \"", columnName, "\" does not exists" });
		}
	}

	MANGO_QUERY_API void SelectQuery::execute(ref<MangoDB> dataBase)
	{
		MangoClauseGuard _(dataBase);
		dataBase.m_LastResult.clear();

		if (m_ColumnNames.empty())
			selectAll(dataBase.getTable(m_TableName), dataBase);
		else
			selectSome(dataBase.getTable(m_TableName), dataBase);
	}
#pragma endregion
	
}