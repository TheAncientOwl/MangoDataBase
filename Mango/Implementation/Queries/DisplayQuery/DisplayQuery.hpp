#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_DISPLAY_SYNTAX "DISPLAY */table_name;"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// DISPLAY */table_name;
	/// </summary>
	class DisplayQuery final : public AbstractQuery
	{
	private:
		std::string m_TableName;

	private:
		MANGO_QUERY_API bool match(std::string_view sql) const override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API DisplayQuery() = default;
		MANGO_PUBLIC_API DisplayQuery(const DisplayQuery&) = default;
		MANGO_PUBLIC_API DisplayQuery(DisplayQuery&&) noexcept = default;
		MANGO_PUBLIC_API DisplayQuery& operator=(const DisplayQuery&) = default;
		MANGO_PUBLIC_API DisplayQuery& operator=(DisplayQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~DisplayQuery() = default;
	};
}