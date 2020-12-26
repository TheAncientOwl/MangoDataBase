#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_SELECT_SYNTAX "SELECT */[col1, col2, ...] FROM (table_name) WHERE <condition>;"

namespace Mango::Queries
{
	/// <summary>
	/// SELECT */[col1, col2, ...] FROM (table_name);
	/// [!] Where clause can be specified through MagoDB::setWhereClause(WhereClause whereClause);
	/// </summary>
	class SelectQuery : public AbstractQuery
	{
	protected:
		std::string m_TableName;
		std::vector<std::string> m_ColumnNames;

	protected:
		/// <summary>
		/// */[] before ()
		/// </summary>
		MANGO_PRIVATE_API void checkStatementsOrder(Statement columns, Statement table, Statement::iterator defaultIt) const;

		/// <summary>
		/// SELECT */[...] FROM (...);
		/// </summary>
		MANGO_PRIVATE_API void checkResidualParts(Statement columns, Statement table, std::string_view sql) const;

		/// <summary>
		/// col1, col2, ...
		/// </summary>
		MANGO_PRIVATE_API void parseColumns(std::string_view columnsPart);

		/// <summary>
		/// table_name
		/// </summary>
		MANGO_PRIVATE_API void parseTableName(std::string_view tablePart);

		MANGO_PRIVATE_API void selectAll(ptr<Table> table, ref<MangoDB> dataBase) const;
		MANGO_PRIVATE_API void selectSome(ptr<Table> table, ref<MangoDB> dataBase) const;

	protected:
		MANGO_QUERY_INTERFACE bool match(std::string_view sql) const final override;
		MANGO_QUERY_INTERFACE void parse(std::string_view sql) override;
		MANGO_QUERY_INTERFACE void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_INTERFACE void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API SelectQuery() = default;
		MANGO_PUBLIC_API SelectQuery(const SelectQuery&) = default;
		MANGO_PUBLIC_API SelectQuery(SelectQuery&&) noexcept = default;
		MANGO_PUBLIC_API SelectQuery& operator=(const SelectQuery&) = default;
		MANGO_PUBLIC_API SelectQuery& operator=(SelectQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~SelectQuery() = default;
	};
}