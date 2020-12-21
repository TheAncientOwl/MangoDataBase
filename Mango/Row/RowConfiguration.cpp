#include "standard_library.hpp"
#include "RowConfiguration.hpp"

namespace Mango
{
	void PRIVATE_API RowConfiguration::pushBack(size_t size, DataType dataType)
	{
		if (!m_Offsets.empty())
			m_Offsets.push_back(m_Offsets.back() + m_Sizes.back());
		else
			m_Offsets.push_back(0);

		m_Sizes.push_back(size);

		m_DataTypes.push_back(dataType);
	}

	size_t PRIVATE_API RowConfiguration::offsetAt(int index) const
	{
		return m_Offsets[index];
	}

	size_t PRIVATE_API RowConfiguration::sizeAt(int index) const
	{
		return m_Sizes[index];
	}

	DataType PRIVATE_API RowConfiguration::dataTypeAt(int index) const
	{
		return m_DataTypes[index];
	}

	size_t RowConfiguration::totalSize() const
	{
		size_t sum = 0;
		for (auto size : m_Sizes)
			sum += size;
		return sum;
	}

}