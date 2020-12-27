#pragma once
#include "../Implementation/Queries/Queries.fwd.hpp"
#include "../Implementation/Row/RowFilters.fwd.hpp"

#include "../Implementation/Table/Table.hpp"

#define MANGO_MAX_TABLE_NAME_LENGTH 50

namespace Mango
{
	namespace Implementation
	{
		std::ostream& operator<<(std::ostream& out, const MangoDB& mango);
	}

	class MangoDB
	{
	private:
		std::filesystem::path m_DirectoryPath;
		std::vector<std::unique_ptr<Implementation::Table>> m_Tables;

		std::vector<Implementation::Row> m_LastResult;

		Implementation::WhereClause m_WhereClause;
		Implementation::SetClause m_SetClause;

	private:
		MANGO_PRIVATE_API std::filesystem::path getConfigFilePath() const;

		/// <summary>
		/// Add table to internal data structure.
		/// Create its files.
		/// Store new configuration.
		/// </summary>
		MANGO_PRIVATE_API void addTable(std::unique_ptr<Implementation::Table> table);

		/// <summary>
		/// Remove table from internal data structure.
		/// Remove its files.
		/// Store new configuration.
		/// </summary>
		MANGO_PRIVATE_API void removeTable(std::string_view tableName);

		MANGO_PRIVATE_API const_ptr<Implementation::Table> getTable(std::string_view tableName) const;
		MANGO_PRIVATE_API ptr<Implementation::Table> getTable(std::string_view tableName);
		MANGO_PRIVATE_API const_ref<std::vector<std::unique_ptr<Implementation::Table>>> tables() const;

		MANGO_PRIVATE_API void storeTableConfigs() const;

	public:
		MANGO_PUBLIC_API void loadTableConfigs();

		MANGO_PUBLIC_API const_ref<std::vector<Implementation::Row>> lastResult();
		MANGO_PUBLIC_API std::vector<Implementation::Row> extractLastResult();
		MANGO_PUBLIC_API void disposeLastResult();

		MANGO_PUBLIC_API void setWhereClause(Implementation::WhereClause whereClause);
		MANGO_PUBLIC_API void resetWhereClause();

		MANGO_PUBLIC_API void setSetClause(Implementation::SetClause setClause);
		MANGO_PUBLIC_API void resetSetClause();
		
	public:
		MANGO_PUBLIC_API MangoDB(std::filesystem::path dataBaseDirectoryPath);

		MANGO_PUBLIC_API MangoDB(const MangoDB&) = delete;
		MANGO_PUBLIC_API MangoDB(MangoDB&&) noexcept = default;
		MANGO_PUBLIC_API MangoDB& operator=(const MangoDB&) = delete;
		MANGO_PUBLIC_API MangoDB& operator=(MangoDB&&) noexcept = default;
		MANGO_PUBLIC_API ~MangoDB();

	private:
		friend class Mango::Implementation::MangoClauseGuard;
		friend class Mango::Implementation::Queries::CreateTableQuery;
		friend class Mango::Implementation::Queries::DropTableQuery;
		friend class Mango::Implementation::Queries::TruncateTableQuery;
		friend class Mango::Implementation::Queries::DisplayQuery;
		friend class Mango::Implementation::Queries::InsertIntoQuery;
		friend class Mango::Implementation::Queries::SelectQuery;
		friend class Mango::Implementation::Queries::DeleteQuery;
		friend class Mango::Implementation::Queries::UpdateQuery;
		friend class Mango::Implementation::Queries::CommandLineAdapter::WhereCondition;
		friend class Mango::Implementation::Queries::CommandLineAdapter::UpdateQueryCLI;
		friend std::ostream& Implementation::operator<<(std::ostream& out, const MangoDB& mango);
	};
}