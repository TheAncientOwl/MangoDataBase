#include "standard_library.hpp"
#include "SelectQuery.hpp"

namespace Mango::Queries
{
	using namespace Mango::Exceptions;

	void SelectQuery::parseColumns(std::string_view columnsPart)
	{
		auto args = splitAtAnyOf(columnsPart, " ,");
		if (args.empty())
			throw InvalidSyntaxException("Since [] are used, column names must be specified");

		for (const auto& arg : args)
			m_ColumnNames.emplace_back(arg);
	}

	void SelectQuery::parseTableName(std::string_view tablePart)
	{
		auto args = splitAtChar(tablePart, ' ');

		if (args.empty())
			throw InvalidSyntaxException("No table specified");

		if (args.size() != 1)
			throw InvalidSyntaxException({ "Unknown sequence \"", tablePart, "\"" });

		m_TableName = args.front();
	}

	void SelectQuery::parseCondition(std::string_view conditionPart)
	{
		if (conditionPart.empty())
			throw InvalidSyntaxException("Empty condition");
	}

	void SelectQuery::checkStatementsOrder(Statement columns, Statement table, Statement condition, Statement::iterator defaultIt)
	{
		columns.checkValidOrder(defaultIt);

		table.checkValidOrder(defaultIt);

		condition.checkValidOrder(defaultIt);

		if (table.open < columns.closed)
			throw InvalidSyntaxException({ "Syntax error, found '", table.openChar, "' before '", columns.closedChar, "'" });

		if (condition.open < table.closed)
			throw InvalidSyntaxException({ "Syntax error, found '", condition.openChar, "' before '", table.closedChar, "'" });
	}

	void SelectQuery::checkResidualParts(Statement columns, Statement table, Statement condition, std::string_view sql)
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
			while (part.starts_with(' '))
				part.remove_prefix(1);
			part.remove_suffix(1);
			if (!part.empty())
				throw InvalidSyntaxException({ "Unexpected sequence \"", part, "\"" });
		}

	}

	void SelectQuery::selectAll(ptr<Table> table, ref<std::vector<Row>> result)
	{
		auto rowConfig = table->makeSharedRowConfiguration();
		size_t rowSize = rowConfig->totalSize();

		TableIterator tableIterator = table->makeIterator();

		result.emplace_back(rowSize, rowConfig);

		while (tableIterator.advanceInPlace(result.back()))
			result.emplace_back(rowSize, rowConfig);
		result.pop_back();
	}

	void SelectQuery::selectSome(ptr<Table> table, ref<std::vector<Row>> result)
	{
		std::vector<int> selectedColumnIndexes;
		auto rowConfig = std::make_shared<RowConfiguration>();

		for (const auto& columnName : m_ColumnNames)
		{
			selectedColumnIndexes.push_back(static_cast<int>(table->getColumnIndex(columnName)));
			const_ref<Column> column = table->getColumn(selectedColumnIndexes.back());

			rowConfig->pushBack(column.size(), column.dataType());
		}

		size_t rowSize = rowConfig->totalSize();

		TableIterator tableIterator = table->makeIterator();
		while (tableIterator.advance())
		{
			auto& row = result.emplace_back(rowSize, rowConfig);

			for (int currentColumn = 0; currentColumn < m_ColumnNames.size(); ++currentColumn)
				row.setDataAt(currentColumn, tableIterator.row().dataAt(selectedColumnIndexes[currentColumn]), rowConfig->sizeAt(currentColumn));
		}
	}

	bool QUERY_API SelectQuery::match(std::string_view sql) const
	{
		return sql.starts_with("SELECT");
	}

	void QUERY_API SelectQuery::parse(std::string_view sql)
	{
		m_TableName.clear();
		m_ColumnNames.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		Statement::iterator DEFAULT = std::cbegin(sql);
		Statement::iterator all = DEFAULT;
		Statement columns(DEFAULT, DEFAULT, "[", "]"), table(DEFAULT, DEFAULT, "(", ")"), condition(DEFAULT, DEFAULT, "<", ">");

		for(auto it = std::cbegin(sql), end = std::cend(sql); it != end; ++it)
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

		checkStatementsOrder(columns, table, condition, DEFAULT);

		checkResidualParts(columns, table, condition, sql);

		if (all == DEFAULT)
			parseColumns({ std::next(columns.open), columns.closed });

		parseTableName({ std::next(table.open), table.closed });

		parseCondition({ std::next(condition.open), condition.closed });

		std::cout << ccolor::dark_gray << "[";
		std::cout << ccolor::gold << "Warn";
		std::cout << ccolor::dark_gray << "] ";
		std::cout << ccolor::gold << "Condition not implemented yet\n";
	}

	void QUERY_API SelectQuery::validate(const_ref<MangoDB> dataBase)
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

	void QUERY_API SelectQuery::execute(ref<MangoDB> dataBase)
	{
		dataBase.m_LastResult.clear();

		if (m_ColumnNames.empty())
			selectAll(dataBase.getTable(m_TableName), dataBase.m_LastResult);
		else
			selectSome(dataBase.getTable(m_TableName), dataBase.m_LastResult);
	}
}