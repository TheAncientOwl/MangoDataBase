#pragma once
namespace Mango
{
	class Table;
	class TableIterator;
	namespace Queries
	{
		class InsertIntoQuery;
		class SelectQuery;
	}
}
#include "RowConfiguration.hpp"

namespace Mango
{
	class Row
	{
	private: /// Fields
		size_t m_Size = 0;
		std::byte* m_Data = nullptr;
		std::shared_ptr<RowConfiguration> m_Config;

	private: /// API
		void PRIVATE_API setDataAt(int index, const void* value, size_t size);
		void PRIVATE_API setDataAt(int index, const_ref<std::string> value);

		const_ptr<std::byte> PRIVATE_API data() const;
		ptr<std::byte> PRIVATE_API data();

		const_ptr<std::byte> PRIVATE_API dataAt(int index) const;
		ptr<std::byte> PRIVATE_API dataAt(int index);

		size_t PRIVATE_API size() const;

	public: /// Interface
		int getInt(int index) const;
		float getFloat(int index) const;
		std::string_view getString(int index) const;

	public: /// Constructors
		Row(size_t size, const_ref<std::shared_ptr<RowConfiguration>> config);

		Row() = default;
		Row(const Row& rhs);
		Row(Row&& rhs) noexcept;
		Row& operator= (const Row& rhs);
		Row& operator= (Row&& rhs) noexcept;
		~Row();

	private: /// Friends
		friend class Mango::Table;
		friend class Mango::TableIterator;
		friend class Mango::Queries::InsertIntoQuery;
		friend class Mango::Queries::SelectQuery;
		//friend RowFilter Mango::RowFilters::Int::getFilter(const_ref<Row> row, int index);
		friend std::ostream& operator<<(std::ostream& out, const Row& row);
	};
}