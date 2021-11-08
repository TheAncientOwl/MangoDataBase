#pragma once
#include "../Queries/Queries.fwd.hpp"
#include "../Row/Row.fwd.hpp"
#include "../Table/Table.fwd.hpp"
#include "../FileIO/CSV/CSV.fwd.hpp"

#include "../DataType/DataType.hpp"

namespace Mango::Implementation
{
	class RowConfiguration
	{
	private:
		/*
			row: 010110011011001101010101011010001010100101
			dataType: int float string string <=> id, salary, last_name, first_name
			sizes:    4   4     50     25
			offsets:  0   4     8      58
		*/
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
		friend class Mango::Implementation::Queries::DisplayQuery;
		friend std::ostream& operator<<(std::ostream& out, const Row& row);
		friend void Mango::Implementation::FileIO::CSV::write(std::ofstream& csv, const_ref<Row> row);
		friend std::ifstream& Mango::Implementation::FileIO::CSV::read(std::ifstream& csv, ref<Row> row);
	};
}