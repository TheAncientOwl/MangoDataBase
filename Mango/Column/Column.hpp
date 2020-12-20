#pragma once
#include "../DataType/DataType.hpp"

#define MANGO_MAX_COLUMN_NAME_LENGTH 50

namespace Mango
{
	class Column
	{
	private:
		std::string m_Name;
		DataType m_DataType;
		size_t m_Size;

	public:
		Column(std::string_view name, DataType dataType, size_t size);

		void serialize(std::fstream& file) const;
		void deserialize(std::fstream& file);

		std::string_view name() const;
		DataType dataType() const;
		size_t size() const;

		Column() = default;
		Column(const Column&) = default;
		Column(Column&&) noexcept = default;
		Column& operator=(const Column&) = default;
		Column& operator=(Column&&) noexcept = default;
		~Column() = default;
	};
}