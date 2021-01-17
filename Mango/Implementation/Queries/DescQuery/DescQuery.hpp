#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_DESC_SYNTAX "DESC */table_name;"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// DESC */table_name;
	/// </summary>
	class DescQuery final : public IQuery
	{
	private:
		std::string m_TableName;

	private:
		MANGO_QUERY_API bool match(std::string_view sql) const override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API DescQuery() = default;
		MANGO_PUBLIC_API DescQuery(const DescQuery&) = default;
		MANGO_PUBLIC_API DescQuery(DescQuery&&) noexcept = default;
		MANGO_PUBLIC_API DescQuery& operator=(const DescQuery&) = default;
		MANGO_PUBLIC_API DescQuery& operator=(DescQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~DescQuery() = default;
	};
}