#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#include "../../Column/Column.hpp"
#define MANGO_CREATE_TABLE_SYNTAX "CREATE TABLE table_name (col1 dataType, col2 dataType, ...);"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// CREATE TABLE table_name (column1 dataType, column2 dataType, ...);
	/// </summary>
	class CreateTableQuery final : public AbstractQuery
	{
	private:
		std::string m_TableName;
		std::vector<Column> m_Columns;

	private:
		/// <summary>
		/// CREATE TABLE table_name
		/// </summary>
		MANGO_API void parseTableName(std::string_view firstPart);

		/// <summary>
		/// column1 dataType, column2 dataType, ...
		/// </summary>
		MANGO_API void parseColumns(std::string_view columnsPart);

		/// <summary>
		/// (...);
		/// residual parts: empty
		/// </summary>
		MANGO_API void checkResidualParts(Statement columns, Statement::iterator stringEnd) const;

	private:
		MANGO_QUERY_API bool match(std::string_view sql) const override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API CreateTableQuery() = default;
		MANGO_PUBLIC_API CreateTableQuery(const CreateTableQuery&) = default;
		MANGO_PUBLIC_API CreateTableQuery(CreateTableQuery&&) noexcept = default;
		MANGO_PUBLIC_API CreateTableQuery& operator=(const CreateTableQuery&) = default;
		MANGO_PUBLIC_API CreateTableQuery& operator=(CreateTableQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~CreateTableQuery() = default;
	};
}