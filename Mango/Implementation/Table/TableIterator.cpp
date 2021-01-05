#include "standard_library.hpp"
#include "TableIterator.hpp"

#include "../FileIO/Binary/Binary.hpp"
using namespace Mango::Implementation::FileIO::Binary;

namespace Mango::Implementation
{
#pragma region MANGO_API
	MANGO_API const_ref<std::fstream> TableIterator::advance()
	{
		deserializePOD(m_FileStream, m_Data.data(), m_Data.size());
		return m_FileStream;
	}

	MANGO_API const_ref<std::fstream> TableIterator::advanceInPlace(ref<Row> row)
	{
		deserializePOD(m_FileStream, row.data(), row.size());
		return m_FileStream;
	}

	MANGO_API const_ref<Row> TableIterator::row() const
	{
		return m_Data;
	}

	ref<Row> TableIterator::row()
	{
		return m_Data;
	}

	MANGO_API void TableIterator::releaseFile()
	{
		m_FileStream.close();
	}

	MANGO_API void TableIterator::overwrite(const_ref<Row> row, bool copyInternal)
	{
		if (copyInternal)
			m_Data = row;

		std::streampos pos = m_FileStream.tellg();
		pos -= row.size();
		m_FileStream.seekp(pos);

		serializePOD(m_FileStream, row.data(), row.size());

		pos += row.size();
		m_FileStream.seekg(pos);
	}

	MANGO_API TableIterator::TableIterator(const_ref<std::filesystem::path> tableDataFilePath, const_ref<std::shared_ptr<RowConfiguration>> rowConfig)
		: m_FileStream(tableDataFilePath, std::ios::in | std::ios::out | std::ios::binary), m_Data(rowConfig->calculateTotalSize(), rowConfig)
	{
	}
#pragma endregion

#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API TableIterator::~TableIterator()
	{
		m_FileStream.close();
	}
#pragma endregion
	
}