#pragma once
#include "../Queries/Queries.fwd.hpp"
#include "../Row/RowFilters.fwd.hpp"

#include "../Table/Table.hpp"

#define MANGO_MAX_TABLE_NAME_LENGTH 50

namespace Mango
{
	class MangoDB
	{
	private:
		std::filesystem::path m_DirectoryPath;
		std::vector<std::unique_ptr<Table>> m_Tables;

		std::vector<Row> m_LastResult;

		WhereClause m_WhereClause;
		SetClause m_SetClause;

	private:
		MANGO_PRIVATE_API std::filesystem::path getConfigFilePath() const;

		/// <summary>
		/// Add table to internal data structure.
		/// Create its files.
		/// Store new configuration.
		/// </summary>
		MANGO_PRIVATE_API void addTable(std::unique_ptr<Table> table);

		/// <summary>
		/// Remove table from internal data structure.
		/// Remove its files.
		/// Store new configuration.
		/// </summary>
		MANGO_PRIVATE_API void removeTable(std::string_view tableName);

		MANGO_PRIVATE_API const_ptr<Table> getTable(std::string_view tableName) const;
		MANGO_PRIVATE_API ptr<Table> getTable(std::string_view tableName);
		MANGO_PRIVATE_API const_ref<std::vector<std::unique_ptr<Table>>> tables() const;

	public:
		MANGO_PUBLIC_API void storeTables() const;
		MANGO_PUBLIC_API void loadTables();

		MANGO_PUBLIC_API const_ref<std::vector<Row>> lastResult();
		MANGO_PUBLIC_API std::vector<Row> extractLastResult();
		MANGO_PUBLIC_API void disposeLastResult();

		MANGO_PUBLIC_API void setWhereClause(WhereClause whereClause);
		MANGO_PUBLIC_API void resetWhereClause();

		MANGO_PUBLIC_API void setSetClause(SetClause setClause);
		MANGO_PUBLIC_API void resetSetClause();
		
	public:
		MANGO_PUBLIC_API MangoDB(std::filesystem::path dataBaseDirectoryPath);

		MANGO_PUBLIC_API MangoDB(const MangoDB&) = delete;
		MANGO_PUBLIC_API MangoDB(MangoDB&&) noexcept = default;
		MANGO_PUBLIC_API MangoDB& operator=(const MangoDB&) = delete;
		MANGO_PUBLIC_API MangoDB& operator=(MangoDB&&) noexcept = default;
		MANGO_PUBLIC_API ~MangoDB() = default;

	private:
		friend class Mango::MangoClauseGuard;
		friend class Mango::Queries::CreateTableQuery;
		friend class Mango::Queries::DropTableQuery;
		friend class Mango::Queries::TruncateTableQuery;
		friend class Mango::Queries::DisplayQuery;
		friend class Mango::Queries::InsertIntoQuery;
		friend class Mango::Queries::SelectQuery;
		friend class Mango::Queries::DeleteQuery;
		friend class Mango::Queries::UpdateQuery;
		friend class Mango::Queries::CommandLineAdapter::SelectQueryCLI;
		friend std::ostream& operator<<(std::ostream& out, const MangoDB& mango);
	};
}