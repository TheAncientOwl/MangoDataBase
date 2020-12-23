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
	private:
		std::string m_TableName;

	private:
		MANGO_QUERY_INTERFACE bool match(std::string_view sql) const override;
		MANGO_QUERY_INTERFACE void parse(std::string_view sql) override;
		MANGO_QUERY_INTERFACE void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_INTERFACE void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API TruncateTableQuery() = default;
		MANGO_PUBLIC_API TruncateTableQuery(const TruncateTableQuery&) = default;
		MANGO_PUBLIC_API TruncateTableQuery(TruncateTableQuery&&) noexcept = default;
		MANGO_PUBLIC_API TruncateTableQuery& operator=(const TruncateTableQuery&) = default;
		MANGO_PUBLIC_API TruncateTableQuery& operator=(TruncateTableQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~TruncateTableQuery() = default;
	};
}