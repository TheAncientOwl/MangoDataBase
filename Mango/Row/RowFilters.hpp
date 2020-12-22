#pragma once
#include "../Row/Row.hpp"

namespace Mango
{
	using RowFilter = bool (*)(const_ref<Row>);
}

namespace Mango::DefaultRowFilters
{
	bool allwaysTrue(const_ref<Row> row);
	bool allwaysFalse(const_ref<Row> row);
}