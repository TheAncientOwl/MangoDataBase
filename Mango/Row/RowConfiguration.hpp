#pragma once
#include "../Queries/Queries.fwd.hpp"
#include "../Row/Row.fwd.hpp"
#include "../Table/Table.fwd.hpp"

#include "../DataType/DataType.hpp"

namespace Mango
{
	class RowConfiguration
	{
	private:
		std::vector<size_t> m_Offsets;
		std::vector<size_t> m_Sizes;
		std::vector<DataType> m_DataTypes;

	private:
		MANGO_PRIVATE_API void pushBack(size_t size, DataType dataType);

		MANGO_PRIVATE_API size_t offsetAt(int index) const;
		MANGO_PRIVATE_API size_t sizeAt(int index) const;
		MANGO_PRIVATE_API DataType dataTypeAt(int index) const;

		MANGO_PRIVATE_API size_t columnsNumber() const;

	public:
		MANGO_PUBLIC_API size_t totalSize() const;

	public:
		MANGO_PUBLIC_API RowConfiguration() = default;
		MANGO_PUBLIC_API RowConfiguration(const RowConfiguration&) = delete;
		MANGO_PUBLIC_API RowConfiguration(RowConfiguration&&) noexcept = default;
		MANGO_PUBLIC_API RowConfiguration& operator=(const RowConfiguration&) = delete;
		MANGO_PUBLIC_API RowConfiguration& operator=(RowConfiguration&&) noexcept = default;
		MANGO_PUBLIC_API ~RowConfiguration() = default;

	private:
		friend class Mango::Row;
		friend class Mango::Table;
		friend class Mango::ConstTableIterator;
		friend class Mango::Queries::SelectQuery;
		friend std::ostream& operator<<(std::ostream& out, const Row& row);
	};
}