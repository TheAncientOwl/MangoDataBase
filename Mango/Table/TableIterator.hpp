#pragma once
namespace Mango
{
	class Table;
}
#include "../Row/Row.hpp"

namespace Mango
{
	class TableIterator
	{
	private: /// Fields
		std::fstream m_FileStream;
		Row m_Data;

	public: /// API
		void PRIVATE_API advance();
		void PRIVATE_API advanceInPlace(ref<Row> row);
		bool PRIVATE_API hasValues() const;
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
	};
}