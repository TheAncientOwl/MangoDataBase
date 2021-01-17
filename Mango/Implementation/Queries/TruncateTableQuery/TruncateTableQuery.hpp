#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_TRUNCATE_TABLE_SYNTAX "TRUNCATE TABLE table_name;"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// TRUNCATE TABLE table_name;
	/// </summary>
	class TruncateTableQuery final : public IQuery
	{
	private:
		std::string m_TableName;

	private:
		MANGO_QUERY_API bool match(std::string_view sql) const override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API TruncateTableQuery() = default;
		MANGO_PUBLIC_API TruncateTableQuery(const TruncateTableQuery&) = default;
		MANGO_PUBLIC_API TruncateTableQuery(TruncateTableQuery&&) noexcept = default;
		MANGO_PUBLIC_API TruncateTableQuery& operator=(const TruncateTableQuery&) = default;
		MANGO_PUBLIC_API TruncateTableQuery& operator=(TruncateTableQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~TruncateTableQuery() = default;
	};
}