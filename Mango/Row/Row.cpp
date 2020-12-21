#include "standard_library.hpp"
#include "Row.hpp"
#include "../Exceptions/InvalidArgumentException/InvalidArgumentException.hpp"

namespace Mango
{
	void PRIVATE_API Row::setDataAt(int index, const void* value, size_t size)
	{
		memcpy(m_Data + m_Config->offsetAt(index), value, size);
	}

	void PRIVATE_API Row::setDataAt(int index, const_ref<std::string> value)
	{
		if (m_Config->dataTypeAt(index) == DataType::Value::INT)
		{
			int dummy = 0;
			try { dummy = std::stoi(value); }
			catch (...)
			{
				throw Exceptions::InvalidArgumentException({"Cannot convert \"", value, "\" to int"});
			}
			setDataAt(index, &dummy, sizeof(int));
		}
		else if (m_Config->dataTypeAt(index) == DataType::Value::FLOAT)
		{
			float dummy = 0;
			try { dummy = std::stof(value); }
			catch (...)
			{
				throw Exceptions::InvalidArgumentException({ "Cannot convert \"", value, "\" to float" });
			}
			setDataAt(index, &dummy, sizeof(float));
		}
		else if (m_Config->dataTypeAt(index) == DataType::Value::STRING)
		{
			if (value.size() >= m_Config->sizeAt(index))
				throw Exceptions::InvalidArgumentException("String argument too long");
			setDataAt(index, value.c_str(), (value.length() + 1) * sizeof(char));
		}
	}

	std::byte* PRIVATE_API Row::data()
	{
		return m_Data;
	}

	int Row::getInt(int index) const
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