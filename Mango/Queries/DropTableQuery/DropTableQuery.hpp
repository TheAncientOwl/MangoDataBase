#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_DROP_TABLE_SYNTAX "DROP TABLE table_name;"

namespace Mango::Queries
{
	/// <summary>
	/// DROP TABLE table_name;
	/// </summary>
	class DropTableQuery final : public AbstractQuery
	{
	private:
		std::string m_TableName;

	private:
		MANGO_QUERY_INTERFACE bool match(std::string_view sql) const override;
		MANGO_QUERY_INTERFACE void parse(std::string_view sql) override;
		MANGO_QUERY_INTERFACE void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_INTERFACE void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API DropTableQuery() = default;
		MANGO_PUBLIC_API DropTableQuery(const DropTableQuery&) = default;
		MANGO_PUBLIC_API DropTableQuery(DropTableQuery&&) noexcept = default;
		MANGO_PUBLIC_API DropTableQuery& operator=(const DropTableQuery&) = default;
		MANGO_PUBLIC_API DropTableQuery& operator=(DropTableQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~DropTableQuery() = default;
	};
}