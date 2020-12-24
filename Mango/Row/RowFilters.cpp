#include "standard_library.hpp"
#include "RowFilters.hpp"

namespace Mango::DefaultRowFilters
{
	bool allwaysTrue(const_ref<Row> row)
	{
		return true;
	}

	bool allwaysFalse(const_ref<Row> row)
	{
		return false;
	}

	void doNothing(ref<Row> row)
	{
		//~NOTHING~ What would you have expected? xD
	}
}