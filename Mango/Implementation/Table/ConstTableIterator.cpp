#include "standard_library.hpp"
#include "ConstTableIterator.hpp"

namespace Mango::Implementation
{
#pragma region MANGO_API
	MANGO_API const_ref<std::fstream> ConstTableIterator::advance()
	{
		return TableIterator::advance();
	}

	MANGO_API const_ref<std::fstream> ConstTableIterator::advanceInPlace(ref<Row> row)
	{
		return TableIterator::advanceInPlace(row);
	}

	MANGO_API const_ref<Row> ConstTableIterator::getRow() const
	{
		return TableIterator::getRow();
	}

	MANGO_API void ConstTableIterator::releaseFile()
	{
		TableIterator::releaseFile();
	}

	ConstTableIterator::ConstTableIterator(const_ref<std::filesystem::path> tableDataFilePath, const_ref<std::shared_ptr<RowConfiguration>> rowConfig)
		: TableIterator(tableDataFilePath, rowConfig)
	{
	}
#pragma endregion

}