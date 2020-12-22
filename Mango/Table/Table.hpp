#pragma once
namespace Mango
{
	class MangoDB;
	namespace Queries
	{
		class TruncateTableQuery;
		class InsertIntoQuery;
	}
}

#include "../Column/Column.hpp"
#include "../Row/Row.hpp"
#include "TableIterator.hpp"

#define MANGO_CONFIG_EXTENSION ".mangocfg"
#define MANGO_DATA_EXTENSION   ".mangodb"

namespace Mango
{
	class Table
	{
	private: /// Fields
		std::string m_Name;
		std::filesystem::path m_DirectoryPath;
		
		std::vector<Column> m_Columns;
		std::map<std::string_view, size_t> m_ColumnIndexes;

	private: /// Private methods
		inline std::filesystem::path getConfigFilePath() const;
		inline std::filesystem::path getDataFilePath() const;

	private: /// API
		void PRIVATE_API createFiles();
		void PRIVATE_API removeFiles();
		void PRIVATE_API clearDataFile();
		void PRIVATE_API serializeConfig();
		void PRIVATE_API deserializeConfig();

		void PRIVATE_API insertRow(const_ref<Row> row);

		const_ref<std::vector<Column>> PRIVATE_API columns() const;
		const_ptr<Column> PRIVATE_API getColumn(std::string_view columnName) const;
		ptr<Column> PRIVATE_API getColumn(std::string_view columnName);
		size_t columnIndex(std::string_view columnName) const;

		std::shared_ptr<RowConfiguration> PRIVATE_API getSharedRowConfiguration() const;
		RowConfiguration PRIVATE_API getRowConfiguration() const;

		TableIterator PRIVATE_API makeIterator();

	public: /// Interface
		std::string_view name() const;

		void TEST() const
		{
			std::fstream file("TestDataBase\\IDK\\IDK.mangodb", std::ios::in | std::ios::binary);

			auto rowConfig = std::make_shared<RowConfiguration>();
			rowConfig->pushBack(sizeof(int), DataType::Value::INT);
			rowConfig->pushBack(sizeof(float), DataType::Value::FLOAT);
			rowConfig->pushBack(40 * sizeof(char), DataType::Value::STRING);

			Row row(rowConfig->totalSize(), rowConfig);

			deserializePOD(file, row.data(), row.size());

			//std::cout << row.getInt(0) << '\n';
			//std::cout << row.getFloat(1) << '\n';
			//std::cout << row.getString(2) << '\n';

			file.close();
		}

	public: /// Constructors
		Table(std::string name, const_ref<std::filesystem::path> dataBaseDirectoryPath, std::vector<Column>&& columns);

		Table() = default;
		Table(const Table&) = delete;
		Table(Table&&) noexcept = default;
		Table& operator=(const Table&) = delete;
		Table& operator=(Table&&) noexcept = default;
		~Table() = default;

	private: /// Friends
		friend class Mango::MangoDB;
		friend class Mango::Queries::TruncateTableQuery;
		friend class Mango::Queries::InsertIntoQuery;
		friend std::ostream& operator<<(std::ostream& out, const Table& table);
	};
}