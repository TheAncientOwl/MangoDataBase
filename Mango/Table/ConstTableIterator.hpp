#pragma once
namespace Mango
{
	class Table;
	namespace Queries
	{
		class SelectQuery;
		class DeleteQuery;
	}
}
#include "TableIterator.hpp"

namespace Mango
{
	class ConstTableIterator : public TableIterator
	{
	private:
		MANGO_PRIVATE_API const_ref<std::fstream> advance();
		MANGO_PRIVATE_API const_ref<std::fstream> advanceInPlace(ref<Row> row);
		MANGO_PRIVATE_API const_ref<Row> row() const;
		MANGO_PRIVATE_API void releaseFile();

	public:
		MANGO_PUBLIC_API ConstTableIterator() = default;
		MANGO_PUBLIC_API ConstTableIterator(const ConstTableIterator&) = delete;
		MANGO_PUBLIC_API ConstTableIterator(ConstTableIterator&&) noexcept = default;
		MANGO_PUBLIC_API ConstTableIterator& operator=(const ConstTableIterator&) = delete;
		MANGO_PUBLIC_API ConstTableIterator& operator=(ConstTableIterator&&) noexcept = default;
		MANGO_PUBLIC_API ~ConstTableIterator() = default;

	private:
		MANGO_PRIVATE_API ConstTableIterator(const_ref<std::filesystem::path> tableDataFilePath, const_ref<std::shared_ptr<RowConfiguration>> rowConfig);

	private:
		friend class Mango::Table;
		friend class Mango::Queries::SelectQuery;
		friend class Mango::Queries::DeleteQuery;
	};
}