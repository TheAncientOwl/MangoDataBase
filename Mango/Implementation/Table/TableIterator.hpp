#pragma once
#include "../Implementation/Queries/Queries.fwd.hpp"
#include "Table.fwd.hpp"

#include "../Row/Row.hpp"

namespace Mango::Implementation
{
	class TableIterator
	{
	private:
		std::fstream m_FileStream;
		Row m_Data;

	private:
		MANGO_PRIVATE_API const_ref<std::fstream> advance();
		MANGO_PRIVATE_API const_ref<std::fstream> advanceInPlace(ref<Row> row);
		MANGO_PRIVATE_API const_ref<Row> row() const;
		MANGO_PRIVATE_API ref<Row> row();
		MANGO_PRIVATE_API void releaseFile();
		MANGO_PRIVATE_API void overwrite(const_ref<Row> row, bool copyInternal = false);

	public:
		MANGO_PUBLIC_API TableIterator() = default;
		MANGO_PUBLIC_API TableIterator(const TableIterator&) = delete;
		MANGO_PUBLIC_API TableIterator(TableIterator&&) noexcept = default;
		MANGO_PUBLIC_API TableIterator& operator=(const TableIterator&) = delete;
		MANGO_PUBLIC_API TableIterator& operator=(TableIterator&&) noexcept = default;
		MANGO_PUBLIC_API ~TableIterator();

	private:
		MANGO_PRIVATE_API TableIterator(const_ref<std::filesystem::path> tableDataFilePath, const_ref<std::shared_ptr<RowConfiguration>> rowConfig);

	private:
		friend class Mango::Implementation::Table;
		friend class Mango::Implementation::ConstTableIterator;
		friend class Mango::Implementation::Queries::UpdateQuery;
	};
}