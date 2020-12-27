#include "standard_library.hpp"
#include "InsertIntoQuery.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries
{
#pragma region MANGO_PRIVATE_API
	MANGO_PRIVATE_API void InsertIntoQuery::checkStatementsOrder(Statement columns, Statement values, Statement::iterator defaultIt) const
	{
		values.checkValidOrder(defaultIt);
		columns.checkValidOrder(defaultIt);
		if (values.open < columns.closed)
			throw InvalidSyntaxException({ "Syntax error, found '", values.openChar, "' before '", columns.closedChar, "'" });
	}

	MANGO_PRIVATE_API void InsertIntoQuery::checkResidualParts(Statement columns, Statement values, Statement::iterator stringEnd) const
	{
		std::string_view valuesKeyword({ std::next(columns.closed), values.open });

		auto args = splitAtChar(valuesKeyword, ' ');
		if (args.empty())
			throw InvalidSyntaxException("Missing \"VALUES\" keyword");

		if (args.size() > 1)
			throw InvalidSyntaxException({ "Unknown sequence \"", valuesKeyword, "\"" });

		if (args.front() != "VALUES")
			throw InvalidArgumentException("Missing \"VALUES\" keyword");

		if (std::string_view end(std::next(values.closed), stringEnd); end.size() > 1)
		{
			while (end.front() == ' ')
				end.remove_prefix(1);
			end.remove_suffix(1);
			if (!end.empty())
				throw InvalidSyntaxException({ "Unexpected query part \"", end, "\"" });
		}
	}

	MANGO_PRIVATE_API void InsertIntoQuery::parseTableName(std::string_view firstPart)
	{
		auto args = splitAtChar(firstPart, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid insert into query syntax");

		if (args[0] != "INSERT" || args[1] != "INTO")
			throw InvalidArgumentException("Check missing \"INSERT\" nor \"INTO\" keywords");

		m_TableName = args[2];
	}

	MANGO_PRIVATE_API void InsertIntoQuery::parseColumnNames(std::string_view columnsPart)
	{
		auto args = splitAtAnyOf(columnsPart, " ,");
		if (args.empty())
			throw InvalidSyntaxException({ "Since \"[]\" are used, column names must be mentioned" });

		for (const auto& columnName : args)
			m_ColumnNames.emplace_back(columnName);
	}

	MANGO_PRIVATE_API void InsertIntoQuery::parseColumnValues(std::string_view valuesPart)
	{
		auto args = splitAtChar(valuesPart, ',');
		if (args.empty())
			throw InvalidSyntaxException({ "No values specified to insert" });

		for (auto& value : args)
		{
			while (!value.empty() && value.front() == ' ')
				value.remove_prefix(1);
			if (value.empty())
				throw InvalidSyntaxException("Empty value");

			while (!value.empty() && value.back() == ' ')
				value.remove_suffix(1);
			if (value.empty())
				throw InvalidSyntaxException("Empty value");

			if (auto it = std::find(std::cbegin(value), std::cend(value), ' ');
				it != std::cend(value) || (value.front() == '"' || value.back() == '"'))//100% string or mistake
			{
				if (value.front() != '"' || value.back() != '"')
					throw InvalidSyntaxException({ "Missing '\"' at \"", value, "\"" });
				value.remove_prefix(1);
				value.remove_suffix(1);
			}

			m_ColumnValues.emplace_back(value);
		}

	}
#pragma endregion

#pragma region MANGO_QUERY_INTERFACE
	MANGO_QUERY_INTERFACE bool InsertIntoQuery::match(std::string_view sql) const
	{
		return sql.starts_with("INSERT");
	}

	MANGO_QUERY_INTERFACE void InsertIntoQuery::parse(std::string_view sql)
	{
		m_InsertAll = false;
		m_TableName.clear();
		m_ColumnNames.clear();
		m_ColumnValues.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		Statement::iterator DEFAULT = std::cbegin(sql);
		Statement columns(DEFAULT, DEFAULT, "[", "]"), values(DEFAULT, DEFAULT, "(", ")");
		Statement::iterator all = DEFAULT;

		for (auto it = std::cbegin(sql), end = std::cend(sql); it != end; ++it)
			switch (*it)
			{
				case ']': columns.closed = it; break;
				case ')': values.closed = it;  break;
				case '[': if (columns.open == DEFAULT) columns.open = it; break;
				case '(': if (values.open == DEFAULT)  values.open = it;  break;
				case '*': all = it; break;
			}

		if (all != DEFAULT)
		{
			if (columns.open != DEFAULT || columns.closed != DEFAULT)
				throw InvalidSyntaxException("Choose between all columns(*) or only some columns");
			m_InsertAll = true;
			columns.open = columns.closed = all;
			columns.openChar = columns.closedChar = "*";
		}

		checkStatementsOrder(columns, values, DEFAULT);

		checkResidualParts(columns, values, std::cend(sql));

		parseTableName({ std::cbegin(sql), columns.open });

		if (columns.openChar != "*")
			parseColumnNames({ std::next(columns.open), columns.closed });

		parseColumnValues({ std::next(values.open), values.closed });
	}

	MANGO_QUERY_INTERFACE void InsertIntoQuery::validate(const_ref<MangoDB> dataBase)
	{
		auto table = dataBase.getTable(m_TableName);
		if (!table)
			throw TableNotFoundException("Table does not exist", std::move(m_TableName));

		if (m_InsertAll)
		{
			if (m_ColumnValues.size() != table->columns().size())
				throw InvalidArgumentException("Number of inserted values must match number of columns in table");
		}
		else
		{
			if (m_ColumnNames.size() != m_ColumnValues.size())
				throw InvalidArgumentException("Number of columns must match number number of inserted values");

			for (const auto& columnName : m_ColumnNames)
				if (!table->getColumn(columnName))
					throw InvalidArgumentException({ "Column \"", columnName, "\" does not exists" });
		}
	}

	MANGO_QUERY_INTERFACE void InsertIntoQuery::execute(ref<MangoDB> dataBase)
	{
		auto table = dataBase.getTable(m_TableName);
		auto rowConfig = table->makeSharedRowConfiguration();
		Row row(rowConfig->calculateTotalSize(), rowConfig);

		std::vector<size_t> columnIndexes;
		columnIndexes.reserve(m_ColumnValues.size());
		if (m_InsertAll)
			for (size_t index = 0; index < m_ColumnValues.size(); ++index)
				columnIndexes.push_back(index);
		else
			for (const auto& columnName : m_ColumnNames)
				columnIndexes.push_back(table->getColumnIndex(columnName));

		for (size_t i = 0; i < m_ColumnValues.size(); ++i)
			row.setDataAt(static_cast<int>(columnIndexes[i]), m_ColumnValues[i]);

		table->insertRow(row);
	}
#pragma endregion

}