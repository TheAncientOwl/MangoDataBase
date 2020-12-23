#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_SELECT_SYNTAX "SELECT */[columns] FROM (table_name) WHERE <condition>;"

namespace Mango::Queries
{
	/// <summary>
	/// SELECT */[columns] FROM (table_name);
	/// [!] Select filter can be specified through MagoDB::setSelectFilter(RowFilter selectFilter);
	/// </summary>
	class SelectQuery final : public AbstractQuery
	{
	private:
		std::string m_TableName;
		std::vector<std::string> m_ColumnNames;

	private:
		/// <summary>
		/// */[] before ()
		/// </summary>
		MANGO_PRIVATE_API void checkStatementsOrder(Statement columns, Statement table, Statement::iterator defaultIt);

		/// <summary>
		/// SELECT */[...] FROM (...);
		/// </summary>
		MANGO_PRIVATE_API void checkResidualParts(Statement columns, Statement table, std::string_view sql);

		/// <summary>
		/// col1, col2, ...
		/// </summary>
		MANGO_PRIVATE_API void parseColumns(std::string_view columnsPart);

		/// <summary>
		/// table_name
		/// </summary>
		MANGO_PRIVATE_API void parseTableName(std::string_view tablePart);

		MANGO_PRIVATE_API void selectAll(ptr<Table> table, ref<std::vector<Row>> result, ref<MangoDB> dataBase);
		MANGO_PRIVATE_API void selectSome(ptr<Table> table, ref<std::vector<Row>> result, ref<MangoDB> dataBase);

	private:
		MANGO_QUERY_INTERFACE bool match(std::string_view sql) const override;
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