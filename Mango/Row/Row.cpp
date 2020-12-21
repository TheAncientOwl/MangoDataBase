#include "standard_library.hpp"
#include "Row.hpp"

namespace Mango
{
	void PRIVATE_API Row::setDataAt(int index, const void* value, size_t size)
	{
		memcpy(m_Data + m_Config->offsetAt(index), value, size);
	}

	std::byte* Row::data()
	{
		return m_Data;
	}

	int PRIVATE_API Row::getInt(int index) const
	{
#ifdef MANGO_ROW_CONFIG_DEBUG
		assert(m_Config->dataTypeAt(index) == DataType::Value::INT);
#endif // MANGO_ROW_CONFIG_DEBUG

		int* dummy = reinterpret_cast<int*>(m_Data + m_Config->offsetAt(index));
		return *dummy;
	}

	float Row::getFloat(int index) const
	{
#ifdef MANGO_ROW_CONFIG_DEBUG
		assert(m_Config->dataTypeAt(index) == DataType::Value::FLOAT);
#endif // MANGO_ROW_CONFIG_DEBUG

		float* dummy = reinterpret_cast<float*>(m_Data + m_Config->offsetAt(index));
		return *dummy;
	}

	std::string_view Row::getString(int index) const
	{
#ifdef MANGO_ROW_CONFIG_DEBUG
		assert(m_Config->dataTypeAt(index) == DataType::Value::STRING);
#endif // MANGO_ROW_CONFIG_DEBUG			

		return std::string_view(reinterpret_cast<char*>(m_Data + m_Config->offsetAt(index)));
	}

	Row::Row(size_t size, const_ref<std::shared_ptr<RowConfiguration>> config)
		: m_Data(new std::byte[size]), m_Config(config)
	{
		memset(m_Data, 0, size);
	}

	Row::Row(Row&& rhs) noexcept
		: m_Data(std::exchange(rhs.m_Data, nullptr)), m_Config(std::move(rhs.m_Config))
	{
	}

	Row& Row::operator=(Row&& rhs) noexcept
	{
		if (this == &rhs)
			return *this;

		m_Data = std::exchange(rhs.m_Data, nullptr);
		m_Config = std::move(rhs.m_Config);

		return *this;
	}

	Row::~Row()
	{
		if (m_Data)
			delete[] m_Data;
	}
}