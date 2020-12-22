#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_SELECT_SYNTAX "SELECT */[columns] FROM (table_name) WHERE <condition>;"

namespace Mango::Queries
{
	/// <summary>
	/// SELECT */[columns] FROM (table_name) WHERE <condition>;
	/// </summary>
	class SelectQuery final : public AbstractQuery
	{
	private: /// Fields
		std::string m_TableName;
		std::vector<std::string> m_ColumnNames;

	private: /// Private Methods
		/// <summary>
		/// col1, col2, ...
		/// </summary>
		void parseColumns(std::string_view columnsPart);

		/// <summary>
		/// table_name
		/// </summary>
		void parseTableName(std::string_view tablePart);

		/// <summary>
		/// condition
		/// </summary>
		void parseCondition(std::string_view conditionPart);

		/// <summary>
		/// */[] before () before <>
		/// </summary>
		void checkStatementsOrder(Statement columns, Statement table, Statement condition, Statement::iterator defaultIt);

		/// <summary>
		/// SELECT */[...] FROM (...) WHERE <...>;
		/// </summary>
		void checkResidualParts(Statement columns, Statement table, Statement condition, std::string_view sql);

		void selectAll(ptr<Table> table, ref<std::vector<Row>> result);
		void selectSome(ptr<Table> table, ref<std::vector<Row>> result);

	private: /// API
		bool QUERY_API match(std::string_view sql) const override;
		void QUERY_API parse(std::string_view sql) override;
		void QUERY_API validate(const_ref<MangoDB> dataBase) override;
		void QUERY_API execute(ref<MangoDB> dataBase) override;

	public: /// Constructors
		SelectQuery() = default;
		SelectQuery(const SelectQuery&) = default;
		SelectQuery(SelectQuery&&) noexcept = default;
		SelectQuery& operator=(const SelectQuery&) = default;
		SelectQuery& operator=(SelectQuery&&) noexcept = default;
		~SelectQuery() = default;
	};
}