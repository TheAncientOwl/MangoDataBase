#pragma once
namespace Mango::Queries
{
	class CreateTableQuery;
	class DropTableQuery;
	class TruncateTableQuery;
	class DisplayQuery;
	class InsertIntoQuery;
	class SelectQuery;
}
#include "../Table/Table.hpp"

#define MANGO_MAX_TABLE_NAME_LENGTH 50

namespace Mango
{
	class MangoDB
	{
	private: /// Fields
		std::filesystem::path m_DirectoryPath;
		std::vector<std::unique_ptr<Table>> m_Tables;

		std::vector<Row> m_LastResult;

	private: /// Private methods
		inline std::filesystem::path getConfigFilePath() const;

	private: /// API
		/// <summary>
		/// Add table to internal data structure.
		/// Create its files.
		/// Store new configuration.
		/// </summary>
		void PRIVATE_API addTable(std::unique_ptr<Table> table);

		/// <summary>
		/// Remove table from internal data structure.
		/// Remove its files.
		/// Store new configuration.
		/// </summary>
		void PRIVATE_API removeTable(std::string_view tableName);

		const_ptr<Table> PRIVATE_API getTable(std::string_view tableName) const;
		ptr<Table> PRIVATE_API getTable(std::string_view tableName);
		const_ref<std::vector<std::unique_ptr<Table>>> PRIVATE_API tables() const;

	public: /// Interface
		void storeTables() const;
		void loadTables();
		const_ref<std::vector<Row>> lastResult();
		std::vector<Row> extractLastResult();
		void disposeLastResult();
		
	public: /// Constructors
		MangoDB(std::filesystem::path dataBaseDirectoryPath);

		MangoDB(const MangoDB&) = delete;
		MangoDB(MangoDB&&) noexcept = default;
		MangoDB& operator=(const MangoDB&) = delete;
		MangoDB& operator=(MangoDB&&) noexcept = default;
		~MangoDB() = default;

	private: /// Friends
		friend class Mango::Queries::CreateTableQuery;
		friend class Mango::Queries::DropTableQuery;
		friend class Mango::Queries::TruncateTableQuery;
		friend class Mango::Queries::DisplayQuery;
		friend class Mango::Queries::InsertIntoQuery;
		friend class Mango::Queries::SelectQuery;
		friend std::ostream& operator<<(std::ostream& out, const MangoDB& mango);
	};
}