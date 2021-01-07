#pragma once
#include "../Row/RowFilters.fwd.hpp"

#include "RowConfiguration.hpp"

namespace Mango::Implementation
{
	class Row
	{
	private:
		size_t m_Size = 0;
		std::byte* m_Data = nullptr;
		std::shared_ptr<RowConfiguration> m_Config;

	private:
		MANGO_API void setDataAt(int index, const void* value, size_t size);
		MANGO_API void setDataAt(int index, const_ref<std::string> value);

		MANGO_API const_ptr<std::byte> data() const;
		MANGO_API ptr<std::byte> data();

		MANGO_API const_ptr<std::byte> dataAt(int index) const;
		MANGO_API ptr<std::byte> dataAt(int index);

		MANGO_API size_t size() const;

	public:
		MANGO_PUBLIC_API int getInt(int index) const;
		MANGO_PUBLIC_API float getFloat(int index) const;
		MANGO_PUBLIC_API std::string_view getString(int index) const;

		MANGO_PUBLIC_API void setInt(int index, int value);
		MANGO_PUBLIC_API void setFloat(int index, float value);
		MANGO_PUBLIC_API void setString(int index, std::string value);
		MANGO_PUBLIC_API void setValue(int index, const_ref<std::string> value);
		MANGO_PUBLIC_API void copyValue(int index, const_ref<Row> row);

	public:
		MANGO_PUBLIC_API Row(size_t size, const_ref<std::shared_ptr<RowConfiguration>> config);

		MANGO_PUBLIC_API Row() = default;
		MANGO_PUBLIC_API Row(const Row& rhs);
		MANGO_PUBLIC_API Row(Row&& rhs) noexcept;
		MANGO_PUBLIC_API Row& operator=(const Row& rhs);
		MANGO_PUBLIC_API Row& operator=(Row&& rhs) noexcept;
		MANGO_PUBLIC_API ~Row();

	private:
		friend class Mango::CommandLineInterface;
		friend class Mango::Implementation::Table;
		friend class Mango::Implementation::TableIterator;
		friend class Mango::Implementation::ConstTableIterator;
		friend class Mango::Implementation::MangoDummyValues;
		friend class Mango::Implementation::Queries::InsertIntoQuery;
		friend class Mango::Implementation::Queries::SelectQuery;
		friend class Mango::Implementation::Queries::DeleteQuery;
		friend void Mango::Implementation::Queries::CommandLineAdapter::setClauseCLI(ref<Row> row);
		friend std::ostream& operator<<(std::ostream& out, const Row& row);
		friend void Mango::Implementation::FileIO::CSV::write(std::ofstream& csv, const_ref<Row> row);
		friend std::ifstream& Mango::Implementation::FileIO::CSV::read(std::ifstream& csv, ref<Row> row);
	};
}