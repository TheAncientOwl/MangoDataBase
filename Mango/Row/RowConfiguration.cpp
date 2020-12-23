#include "standard_library.hpp"
#include "RowConfiguration.hpp"

namespace Mango
{
#pragma region MANGO_PRIVATE_API
	MANGO_PRIVATE_API void RowConfiguration::pushBack(size_t size, DataType dataType)
	{
		if (!m_Offsets.empty())
			m_Offsets.push_back(m_Offsets.back() + m_Sizes.back());
		else
			m_Offsets.push_back(0);

		m_Sizes.push_back(size);

		m_DataTypes.push_back(dataType);
	}

	MANGO_PRIVATE_API size_t RowConfiguration::offsetAt(int index) const
	{
		assert(index >= 0 && index < m_Offsets.size());
		return m_Offsets[index];
	}

	MANGO_PRIVATE_API size_t RowConfiguration::sizeAt(int index) const
	{
		assert(index >= 0 && index < m_Sizes.size());
		return m_Sizes[index];
	}

	MANGO_PRIVATE_API DataType RowConfiguration::dataTypeAt(int index) const
	{
		assert(index >= 0 && index < m_Sizes.size());
		return m_DataTypes[index];
	}

	MANGO_PRIVATE_API size_t RowConfiguration::columnsNumber() const
	{
		return m_Sizes.size();
	}
#pragma endregion

#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API size_t RowConfiguration::totalSize() const
	{
		size_t sum = 0;
		for (auto size : m_Sizes)
			sum += size;
		return sum;
	}
#pragma endregion

}