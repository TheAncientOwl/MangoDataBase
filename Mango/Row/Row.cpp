#include "standard_library.hpp"
#include "Row.hpp"
#include "../Exceptions/InvalidArgumentException/InvalidArgumentException.hpp"

namespace Mango
{
#pragma region MANGO_PRIVATE_API
	MANGO_PRIVATE_API void Row::setDataAt(int index, const void* value, size_t size)
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		memcpy(m_Data + m_Config->offsetAt(index), value, size);
	}

	MANGO_PRIVATE_API void Row::setDataAt(int index, const_ref<std::string> value)
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		if (m_Config->dataTypeAt(index) == DataType::Value::INT)
		{
			int dummy = 0;
			try { dummy = std::stoi(value); }
			catch (...)
			{
				throw Exceptions::InvalidArgumentException({ "Cannot convert \"", value, "\" to int" });
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

	MANGO_PRIVATE_API const_ptr<std::byte> Row::data() const
	{
		return m_Data;
	}

	MANGO_PRIVATE_API ptr<std::byte> Row::data()
	{
		return m_Data;
	}

	MANGO_PRIVATE_API const_ptr<std::byte> Row::dataAt(int index) const
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		return m_Data + m_Config->offsetAt(index);
	}

	MANGO_PRIVATE_API ptr<std::byte> Row::dataAt(int index)
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		return m_Data + m_Config->offsetAt(index);
	}

	MANGO_PRIVATE_API size_t Row::size() const
	{
		return m_Size;
	}
#pragma endregion

#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API int Row::getInt(int index) const
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		assert(m_Config->dataTypeAt(index) == DataType::Value::INT);

		int* dummy = reinterpret_cast<int*>(m_Data + m_Config->offsetAt(index));
		return *dummy;
	}

	MANGO_PUBLIC_API float Row::getFloat(int index) const
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		assert(m_Config->dataTypeAt(index) == DataType::Value::FLOAT);

		float* dummy = reinterpret_cast<float*>(m_Data + m_Config->offsetAt(index));
		return *dummy;
	}

	MANGO_PUBLIC_API std::string_view Row::getString(int index) const
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		assert(m_Config->dataTypeAt(index) == DataType::Value::STRING);

		return std::string_view(reinterpret_cast<char*>(m_Data + m_Config->offsetAt(index)));
	}

	MANGO_PUBLIC_API void Row::setInt(int index, int value)
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		assert(m_Config->dataTypeAt(index) == DataType::Value::INT);

		setDataAt(index, &value, sizeof(int));
	}

	MANGO_PUBLIC_API void Row::setFloat(int index, float value)
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		assert(m_Config->dataTypeAt(index) == DataType::Value::FLOAT);

		setDataAt(index, &value, sizeof(float));
	}

	MANGO_PUBLIC_API void Row::setString(int index, std::string value)
	{
		assert(index >= 0 && index < m_Config->columnsNumber());
		assert(m_Config->dataTypeAt(index) == DataType::Value::STRING);

		if (value.size() >= m_Config->sizeAt(index))
			throw Exceptions::InvalidArgumentException("String argument too long");
		setDataAt(index, value.c_str(), (value.length() + 1) * sizeof(char));
	}

	MANGO_PUBLIC_API void Row::setValue(int index, const_ref<std::string> value)
	{
		assert(index >= 0 && index < m_Config->columnsNumber());

		setDataAt(index, value);
	}

	MANGO_PUBLIC_API Row::Row(size_t size, const_ref<std::shared_ptr<RowConfiguration>> config)
		: m_Size(size), m_Data(new std::byte[size]), m_Config(config)
	{
		memset(m_Data, 0, size);
	}

	MANGO_PUBLIC_API Row::Row(const Row& rhs) : m_Config(rhs.m_Config)
	{
		m_Size = rhs.m_Size;

		m_Data = new std::byte[m_Size];
		memcpy(m_Data, rhs.m_Data, m_Size);
	}

	MANGO_PUBLIC_API Row::Row(Row&& rhs) noexcept
	{
		m_Size = std::exchange(rhs.m_Size, 0);
		m_Data = std::exchange(rhs.m_Data, nullptr);
		m_Config = std::move(rhs.m_Config);
	}

	MANGO_PUBLIC_API Row& Row::operator=(const Row& rhs)
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

		return *this;
	}

	MANGO_PUBLIC_API Row& Row::operator=(Row&& rhs) noexcept
	{
		if (this == &rhs)
			return *this;

		m_Size = std::exchange(rhs.m_Size, 0);
		m_Data = std::exchange(rhs.m_Data, nullptr);
		m_Config = std::move(rhs.m_Config);

		return *this;
	}

	MANGO_PUBLIC_API Row::~Row()
	{
		if (m_Data)
			delete[] m_Data;
	}

	MANGO_PUBLIC_API std::ostream& operator<<(std::ostream& out, const Row& row)
	{
		out << ccolor::dark_gray << "| ";
		for (int columnsNumber = static_cast<int>(row.m_Config->columnsNumber()), index = 0; index < columnsNumber; ++index)
		{
			switch (row.m_Config->dataTypeAt(index))
			{
			case DataType::Value::INT: out << ccolor::green << row.getInt(index) << ccolor::dark_gray << " | "; break;
			case DataType::Value::FLOAT: out << ccolor::green << row.getFloat(index) << ccolor::dark_gray << " | "; break;
			case DataType::Value::STRING: out << ccolor::green << row.getString(index) << ccolor::dark_gray << " | "; break;
			}
		}

		return out;
	}
#pragma endregion

}