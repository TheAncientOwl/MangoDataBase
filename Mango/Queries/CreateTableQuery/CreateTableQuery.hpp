#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#include "../../Column/Column.hpp"
#define MANGO_CREATE_TABLE_SYNTAX "CREATE TABLE table_name (col1 dataType, col2 dataType, ...);"

namespace Mango::Queries
{
	/// <summary>
	/// CREATE TABLE table_name
	/// (
	///		column1 dataType,
	///		column2 dataType,
	///		column3 dataType,
	///		...
	/// );
	/// </summary>
	class CreateTableQuery final : public AbstractQuery
	{
	private: /// Fields
		std::string m_TableName;
		std::vector<Column> m_Columns;

	private: /// Private Methods
		/// <summary>
		/// CREATE TABLE table_name
		/// </summary>
		void parseTableName(std::string_view firstPart);

		/// <summary>
		/// (
		///		column1 dataType,
		///		column2 dataType,
		///		column3 dataType,
		///		...
		/// ) 
		/// </summary>
		void parseColumns(std::string_view columnsPart);

		/// <summary>
		/// (...);
		/// residual parts: empty
		/// </summary>
		void checkResidualParts(Statement columns, Statement::iterator stringEnd);

	private: /// API
		bool QUERY_API match(std::string_view sql) const override;
		void QUERY_API parse(std::string_view sql) override;
		void QUERY_API validate(const_ref<MangoDB> dataBase) override;
		void QUERY_API execute(ref<MangoDB> dataBase) override;

	public: /// Constructors
		CreateTableQuery() = default;
		CreateTableQuery(const CreateTableQuery&) = default;
		CreateTableQuery(CreateTableQuery&&) noexcept = default;
		CreateTableQuery& operator=(const CreateTableQuery&) = default;
		CreateTableQuery& operator=(CreateTableQuery&&) noexcept = default;
		~CreateTableQuery() = default;
	};
}