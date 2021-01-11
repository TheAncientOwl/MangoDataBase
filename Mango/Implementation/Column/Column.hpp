#pragma once
#include "../Table/Table.fwd.hpp"

#include "../DataType/DataType.hpp"

#define MANGO_MAX_COLUMN_NAME_LENGTH 50

namespace Mango::Implementation
{
	class Column
	{
	private:
		std::string m_Name;
		DataType m_DataType;
		size_t m_Size = 0;

	private:
		MANGO_API void serializeToConfig(std::fstream& config) const;
		MANGO_API void deserializeFromConfig(std::fstream& config);

	public:
		MANGO_PUBLIC_API std::string_view getName() const;
		MANGO_PUBLIC_API DataType getDataType() const;
		MANGO_PUBLIC_API size_t getSize() const;

	public:
		MANGO_PUBLIC_API Column(std::string_view name, DataType dataType, size_t size);

		MANGO_PUBLIC_API Column() = default;
		MANGO_PUBLIC_API Column(const Column&) = default;
		MANGO_PUBLIC_API Column(Column&&) noexcept = default;
		MANGO_PUBLIC_API Column& operator=(const Column&) = default;
		MANGO_PUBLIC_API Column& operator=(Column&&) noexcept = default;
		MANGO_PUBLIC_API ~Column() = default;

	private:
		friend class Mango::Implementation::Table;
		friend std::ostream& operator<<(std::ostream& out, const Column& col);
	};
}