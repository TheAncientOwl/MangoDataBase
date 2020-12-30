#pragma once
#include "../Queries/Queries.fwd.hpp"
#include "Table.fwd.hpp"

#include "TableIterator.hpp"

namespace Mango::Implementation
{
	class ConstTableIterator : private TableIterator
	{
	private:
		MANGO_API const_ref<std::fstream> advance();
		MANGO_API const_ref<std::fstream> advanceInPlace(ref<Row> row);
		MANGO_API const_ref<Row> row() const;
		MANGO_API void releaseFile();

	public:
		MANGO_PUBLIC_API ConstTableIterator() = default;
		MANGO_PUBLIC_API ConstTableIterator(const ConstTableIterator&) = delete;
		MANGO_PUBLIC_API ConstTableIterator(ConstTableIterator&&) noexcept = default;
		MANGO_PUBLIC_API ConstTableIterator& operator=(const ConstTableIterator&) = delete;
		MANGO_PUBLIC_API ConstTableIterator& operator=(ConstTableIterator&&) noexcept = default;
		MANGO_PUBLIC_API ~ConstTableIterator() = default;

	private:
		MANGO_API ConstTableIterator(const_ref<std::filesystem::path> tableDataFilePath, const_ref<std::shared_ptr<RowConfiguration>> rowConfig);

	private:
		friend class Mango::Implementation::Table;
		friend class Mango::Implementation::Queries::SelectQuery;
		friend class Mango::Implementation::Queries::DeleteQuery;
	};
}