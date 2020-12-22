#include "standard_library.hpp"
#include "RowFilters.hpp"

namespace Mango::RowFilters
{
	bool allwaysTrue(const_ref<Row> row)
	{
		return true;
	}

	bool allwaysFalse(const_ref<Row> row)
	{
		return false;
	}
}