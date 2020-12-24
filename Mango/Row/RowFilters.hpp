#pragma once
#include "../Row/Row.hpp"

namespace Mango
{
	/// <summary>
	/// Pointer to a function which takes a row, evaluates the row for the where clause.
	/// The functions should return true if the row should be used in clause.
	/// SelectQuery -> true if the row should be selected, false otherwise;
	/// DeleteQuery -> true if the row should be deleted, false otherwise;
	/// UpdateQuery -> true if the rou should be updated, false otherwise;
	/// </summary>
	using WhereClause = bool (*)(const_ref<Row>);

	/// <summary>
	/// Pointer to function which takes a ref to a row and modifies it
	/// in order to store it into thee database.
	/// </summary>
	using SetClause = void (*)(ref<Row>);
}

namespace Mango::DefaultRowFilters
{
	bool allwaysTrue(const_ref<Row> row);
	bool allwaysFalse(const_ref<Row> row);
	void doNothing(ref<Row> row);
}