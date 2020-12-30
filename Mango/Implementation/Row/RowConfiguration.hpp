#pragma once
#include "../Queries/Queries.fwd.hpp"
#include "../Row/Row.fwd.hpp"
#include "../Table/Table.fwd.hpp"

#include "../DataType/DataType.hpp"

namespace Mango::Implementation
{
	class RowConfiguration
	{
	private:
		std::vector<size_t> m_Offsets;
		std::vector<size_t> m_Sizes;
		std::vector<DataType> m_DataTypes;

	private:
		MANGO_API void pushBack(size_t size, DataType dataType);

		MANGO_API size_t offsetAt(int index) const;
		MANGO_API size_t sizeAt(int index) const;
		MANGO_API DataType dataTypeAt(int index) const;

		MANGO_API size_t columnsNumber() const;

	public:
		MANGO_PUBLIC_API size_t calculateTotalSize() const;
		MANGO_PUBLIC_API bool operator==(const RowConfiguration& rhs) const;

	public:
		MANGO_PUBLIC_API RowConfiguration() = default;
		MANGO_PUBLIC_API RowConfiguration(const RowConfiguration&) = delete;
		MANGO_PUBLIC_API RowConfiguration(RowConfiguration&&) noexcept = default;
		MANGO_PUBLIC_API RowConfiguration& operator=(const RowConfiguration&) = delete;
		MANGO_PUBLIC_API RowConfiguration& operator=(RowConfiguration&&) noexcept = default;
		MANGO_PUBLIC_API ~RowConfiguration() = default;

	private:
		friend class Mango::Implementation::Row;
		friend class Mango::Implementation::Table;
		friend class Mango::Implementation::ConstTableIterator;
		friend class Mango::Implementation::Queries::SelectQuery;
		friend class Mango::Implementation::Queries::CommandLineAdapter::SetClauseData;
		friend std::ostream& operator<<(std::ostream& out, const Row& row);
	};
}