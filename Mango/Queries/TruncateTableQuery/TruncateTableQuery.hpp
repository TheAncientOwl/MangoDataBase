#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_DROP_TABLE_SYNTAX "DROP TABLE table_name"

namespace Mango::Queries
{
	/// <summary>
	/// TRUNCATE TABLE table_name;
	/// </summary>
	class TruncateTableQuery final : public AbstractQuery
	{
	private: /// Fields
		std::string m_TableName;

	private: /// API
		bool QUERY_API match(std::string_view sql) const override;
		void QUERY_API parse(std::string_view sql) override;
		void QUERY_API validate(const_ref<MangoDB> dataBase) override;
		void QUERY_API execute(ref<MangoDB> dataBase) override;

	public: /// Constructors
		TruncateTableQuery() = default;
		TruncateTableQuery(const TruncateTableQuery&) = default;
		TruncateTableQuery(TruncateTableQuery&&) noexcept = default;
		TruncateTableQuery& operator=(const TruncateTableQuery&) = default;
		TruncateTableQuery& operator=(TruncateTableQuery&&) noexcept = default;
		~TruncateTableQuery() = default;
	};
}