#pragma once
namespace Mango
{
	class Table;
}

#include "../DataType/DataType.hpp"

#define MANGO_MAX_COLUMN_NAME_LENGTH 50

namespace Mango
{
	class Column
	{
	private: /// Fields
		std::string m_Name;
		DataType m_DataType;
		size_t m_Size;

	private: /// API
		void PRIVATE_API serialize(std::fstream& file) const;
		void PRIVATE_API deserialize(std::fstream& file);

	public: /// Interface
		std::string_view name() const;
		DataType dataType() const;
		size_t size() const;

	public: /// Constructors
		Column(std::string_view name, DataType dataType, size_t size);

		Column() = default;
		Column(const Column&) = default;
		Column(Column&&) noexcept = default;
		Column& operator=(const Column&) = default;
		Column& operator=(Column&&) noexcept = default;
		~Column() = default;

	private: /// Friends
		friend class Mango::Table;
		friend std::ostream& operator<<(std::ostream& out, const Column& col);
	};
}