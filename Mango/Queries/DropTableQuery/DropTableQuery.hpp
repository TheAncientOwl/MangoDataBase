#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_DROP_TABLE_SYNTAX "DROP TABLE table_name"

namespace Mango::Queries
{
	class DropTableQuery final : public AbstractQuery
	{
	private: /// Fields
		std::string m_TableName;

	private: /// API
		bool QUERY_API match(std::string_view sql) const override;
		void QUERY_API parse(std::string_view sql) override;
		void QUERY_API validate(const_ref<MangoDB> dataBase) override;
		void QUERY_API execute(ref<MangoDB> dataBase) override;

	public: /// Constructors
		DropTableQuery() = default;
		DropTableQuery(const DropTableQuery&) = default;
		DropTableQuery(DropTableQuery&&) noexcept = default;
		DropTableQuery& operator=(const DropTableQuery&) = default;
		DropTableQuery& operator=(DropTableQuery&&) noexcept = default;
		~DropTableQuery() = default;
	};
}