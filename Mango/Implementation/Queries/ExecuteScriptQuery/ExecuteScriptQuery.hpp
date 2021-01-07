#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_EXECUTE_SCRIPT_QUERY "EXECUTE SCRIPT script.mango;"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// EXECUTE SCRIPT script.mango;
	/// </summary>
	class ExecuteScriptQuery final : public AbstractQuery
	{
	private:
		std::string m_ScriptName;

	private:
		MANGO_QUERY_API bool match(std::string_view sql) const override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API ExecuteScriptQuery() = default;
		MANGO_PUBLIC_API ExecuteScriptQuery(const ExecuteScriptQuery&) = default;
		MANGO_PUBLIC_API ExecuteScriptQuery(ExecuteScriptQuery&&) noexcept = default;
		MANGO_PUBLIC_API ExecuteScriptQuery& operator=(const ExecuteScriptQuery&) = default;
		MANGO_PUBLIC_API ExecuteScriptQuery& operator=(ExecuteScriptQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~ExecuteScriptQuery() = default;
	};
}