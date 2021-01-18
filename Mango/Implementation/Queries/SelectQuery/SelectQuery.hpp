#pragma once
#include "../../Row/RowFilters.hpp"
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_SELECT_SYNTAX "SELECT */[col1, col2, ...] FROM table_name;"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// SELECT */[col1, col2, ...] FROM table_name;
	/// [!] Where clause can be specified through MagoDB::setWhereClause(WhereClause whereClause);
	/// </summary>
	class SelectQuery : public AbstractQuery
	{
	protected:
		std::string m_TableName;
		std::vector<std::string> m_ColumnNames;

	protected:
		/// <summary>
		/// col1, col2, ...
		/// </summary>
		MANGO_API void parseColumnNames(std::string_view columnsPart);

		/// <summary>
		/// FROM table_name
		/// </summary>
		MANGO_API void parseTableName(std::string_view tablePart);

		MANGO_API void selectAll(ptr<Table> table, ref<MangoDB> dataBase);
		MANGO_API void selectSome(ptr<Table> table, ref<MangoDB> dataBase);

	protected:
		MANGO_QUERY_API bool match(std::string_view sql) const final override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API SelectQuery() = default;
		MANGO_PUBLIC_API SelectQuery(const SelectQuery&) = default;
		MANGO_PUBLIC_API SelectQuery(SelectQuery&&) noexcept = default;
		MANGO_PUBLIC_API SelectQuery& operator=(const SelectQuery&) = default;
		MANGO_PUBLIC_API SelectQuery& operator=(SelectQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~SelectQuery() = default;
	};
}