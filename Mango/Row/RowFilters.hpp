#pragma once
#include "../Row/Row.hpp"

namespace Mango
{
	/// <summary>
	/// Pointer to a function which takes a row, evaluates the row for the where clause.
	/// The functions should return true if the row should be used in clause.
	/// SelectQuery -> true if the row should be selected, false otherwise;
	/// DeleteQuery -> true if the row should be deleted, false otherwise;
	/// </summary>
	using WhereClause = bool (*)(const_ref<Row>);
}

namespace Mango::DefaultRowFilters
{
	bool allwaysTrue(const_ref<Row> row);
	bool allwaysFalse(const_ref<Row> row);
}