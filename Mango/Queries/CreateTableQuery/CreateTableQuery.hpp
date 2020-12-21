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

	private: /// Class Methods
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

	private: /// Overidden Methods
		bool QUERY_INTERFACE match(std::string_view sql) const override;
		void QUERY_INTERFACE parse(std::string_view sql) override;
		void QUERY_INTERFACE validate(const_ref<MangoDB> dataBase) override;
		void QUERY_INTERFACE execute(ref<MangoDB> dataBase) override;

	public:
		CreateTableQuery() = default;
		CreateTableQuery(const CreateTableQuery&) = delete;
		CreateTableQuery(CreateTableQuery&&) noexcept = delete;
		CreateTableQuery& operator=(const CreateTableQuery&) = delete;
		CreateTableQuery& operator=(CreateTableQuery&&) noexcept = delete;
		~CreateTableQuery() = default;
	};
}