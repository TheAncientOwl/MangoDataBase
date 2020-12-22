#pragma once
namespace Mango
{
	class Table;
	namespace Queries
	{
		class SelectQuery;
	}
}
#include "../Row/Row.hpp"

namespace Mango
{
	class TableIterator
	{
	private: /// Fields
		std::fstream m_FileStream;
		Row m_Data;

	private: /// API
		const_ref<std::fstream> PRIVATE_API advance();
		const_ref<std::fstream> PRIVATE_API advanceInPlace(ref<Row> row);
		const_ref<Row> PRIVATE_API row();

	public: /// Constructors
		TableIterator() = default;
		TableIterator(const TableIterator&) = delete;
		TableIterator(TableIterator&&) noexcept = default;
		TableIterator& operator=(const TableIterator&) = delete;
		TableIterator& operator=(TableIterator&&) noexcept = default;
		~TableIterator();

	private:
		TableIterator(const_ref<std::filesystem::path> tableDataFilePath, const_ref<std::shared_ptr<RowConfiguration>> rowConfig);

	private: /// Friends
		friend class Mango::Table;
		friend class Mango::Queries::SelectQuery;
	};
}