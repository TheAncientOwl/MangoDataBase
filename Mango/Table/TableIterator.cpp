#include "standard_library.hpp"
#include "TableIterator.hpp"

namespace Mango
{
	void PRIVATE_API TableIterator::advance()
	{
		deserializePOD(m_FileStream, m_Data.data(), m_Data.size());
	}

	void TableIterator::advanceInPlace(ref<Row> row)
	{
		deserializePOD(m_FileStream, row.data(), row.size());
	}

	bool TableIterator::hasValues() const
	{
		return !m_FileStream.eof();
	}

	const_ref<Row> PRIVATE_API TableIterator::row()
	{
		return m_Data;
	}

	TableIterator::TableIterator(const_ref<std::filesystem::path> tableDataFilePath, const_ref<std::shared_ptr<RowConfiguration>> rowConfig)
		: m_FileStream(tableDataFilePath, std::ios::in | std::ios::binary), m_Data(rowConfig->totalSize(), rowConfig)
	{
		advance();
	}

	TableIterator::~TableIterator()
	{
		m_FileStream.close();
	}

}