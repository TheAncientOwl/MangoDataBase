#pragma once
namespace Mango
{
	class Table;
	class TableIterator;
	class ConstTableIterator;
	namespace Queries
	{
		class InsertIntoQuery;
		class SelectQuery;
		class DeleteQuery;
	}
}
#include "RowConfiguration.hpp"

namespace Mango
{
	class Row
	{
	private:
		size_t m_Size = 0;
		std::byte* m_Data = nullptr;
		std::shared_ptr<RowConfiguration> m_Config;

	private:
		MANGO_PRIVATE_API void setDataAt(int index, const void* value, size_t size);
		MANGO_PRIVATE_API void setDataAt(int index, const_ref<std::string> value);

		MANGO_PRIVATE_API const_ptr<std::byte> data() const;
		MANGO_PRIVATE_API ptr<std::byte> data();

		MANGO_PRIVATE_API const_ptr<std::byte> dataAt(int index) const;
		MANGO_PRIVATE_API ptr<std::byte> dataAt(int index);

		MANGO_PRIVATE_API size_t size() const;

	public:
		MANGO_PUBLIC_API int getInt(int index) const;
		MANGO_PUBLIC_API float getFloat(int index) const;
		MANGO_PUBLIC_API std::string_view getString(int index) const;

	public:
		MANGO_PUBLIC_API Row(size_t size, const_ref<std::shared_ptr<RowConfiguration>> config);

		MANGO_PUBLIC_API Row() = default;
		MANGO_PUBLIC_API Row(const Row& rhs);
		MANGO_PUBLIC_API Row(Row&& rhs) noexcept;
		MANGO_PUBLIC_API Row& operator=(const Row& rhs);
		MANGO_PUBLIC_API Row& operator=(Row&& rhs) noexcept;
		MANGO_PUBLIC_API ~Row();

	private:
		friend class Mango::Table;
		friend class Mango::TableIterator;
		friend class Mango::ConstTableIterator;
		friend class Mango::Queries::InsertIntoQuery;
		friend class Mango::Queries::SelectQuery;
		friend class Mango::Queries::DeleteQuery;
		friend std::ostream& operator<<(std::ostream& out, const Row& row);
	};
}