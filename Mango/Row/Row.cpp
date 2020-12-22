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

	const_ptr<std::byte> PRIVATE_API Row::data() const
	{
		return m_Data;
	}

	ptr<std::byte> PRIVATE_API Row::data()
	{
		return m_Data;
	}

	size_t Row::size() const
	{
		return m_Size;
	}

	int Row::getInt(int index) const
	{
		assert(m_Config->dataTypeAt(index) == DataType::Value::INT);
		int* dummy = reinterpret_cast<int*>(m_Data + m_Config->offsetAt(index));
		return *dummy;
	}

	float Row::getFloat(int index) const
	{
		assert(m_Config->dataTypeAt(index) == DataType::Value::FLOAT);
		float* dummy = reinterpret_cast<float*>(m_Data + m_Config->offsetAt(index));
		return *dummy;
	}

	std::string_view Row::getString(int index) const
	{
		assert(m_Config->dataTypeAt(index) == DataType::Value::STRING);
		return std::string_view(reinterpret_cast<char*>(m_Data + m_Config->offsetAt(index)));
	}

	Row::Row(size_t size, const_ref<std::shared_ptr<RowConfiguration>> config)
		: m_Size(size), m_Data(new std::byte[size]), m_Config(config)
	{
		memset(m_Data, 0, size);
	}

	Row::Row(const Row& rhs) : m_Config(rhs.m_Config)
	{
		m_Size = rhs.m_Size;

		m_Data = new std::byte[m_Size];
		memcpy(m_Data, rhs.m_Data, m_Size);
	}

	Row::Row(Row&& rhs) noexcept
	{
		m_Size = std::exchange(rhs.m_Size, 0);
		m_Data = std::exchange(rhs.m_Data, nullptr);
		m_Config = std::move(rhs.m_Config);
	}

	Row& Row::operator=(const Row& rhs)
	{
		if (this == &rhs)
			return *this;

		if (m_Size != rhs.m_Size)
		{
			delete[] m_Data;
			m_Data = new std::byte[rhs.m_Size];
		}

		m_Size = rhs.m_Size;
		memcpy(m_Data, rhs.m_Data, m_Size);
		m_Config = rhs.m_Config;

		return* this;
	}

	Row& Row::operator=(Row&& rhs) noexcept
	{
		if (this == &rhs)
			return *this;

		m_Size = std::exchange(rhs.m_Size, 0);
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