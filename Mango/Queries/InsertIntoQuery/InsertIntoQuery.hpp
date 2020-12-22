#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_INSERT_INTO_SYNTAX "INSERT INTO table_name [columns] VALUES (val1, val2, ...);"

namespace Mango::Queries
{
	/// <summary>
	/// INSERT INTO table_name [columns] VALUES (val1, val2, ...);
	/// </summary>
	class InsertIntoQuery final : public AbstractQuery
	{
	private: /// Fields
		bool m_InsertAll;
		std::string m_TableName;
		std::vector<std::string> m_ColumnNames;
		std::vector<std::string> m_ColumnValues;

	private: /// Private Methods
		/// <summary>
		/// INSERT INTO table_name
		/// </summary>
		void parseTableName(std::string_view firstPart);

		/// <summary>
		/// col1, col2, col3, ...
		/// </summary>
		void parseColumnNames(std::string_view columnsPart);

		/// <summary>
		/// val1, val2, val3, ...
		/// </summary>
		void parseColumnValues(std::string_view valuesPart);

		/// <summary>
		/// [] before ()
		/// </summary>
		void checkStatementsOrder(Statement columns, Statement values, Statement::iterator defaultIt);

		/// <summary>
		/// allowed: VALUES after [] optionally.
		/// </summary>
		void checkResidualParts(Statement columns, Statement values, Statement::iterator stringEnd);

	private: /// API
		bool QUERY_API match(std::string_view sql) const override;
		void QUERY_API parse(std::string_view sql) override;
		void QUERY_API validate(const_ref<MangoDB> dataBase) override;
		void QUERY_API execute(ref<MangoDB> dataBase) override;

	public: /// Constructors
		InsertIntoQuery() = default;
		InsertIntoQuery(const InsertIntoQuery&) = default;
		InsertIntoQuery(InsertIntoQuery&&) noexcept = default;
		InsertIntoQuery& operator=(const InsertIntoQuery&) = default;
		InsertIntoQuery& operator=(InsertIntoQuery&&) noexcept = default;
		~InsertIntoQuery() = default;
	};
}