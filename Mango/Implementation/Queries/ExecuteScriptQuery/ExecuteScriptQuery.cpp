#include "standard_library.hpp"
#include "ExecuteScriptQuery.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../../StringUtils/StringUtils.hpp"

namespace Mango::Implementation::Queries
{
#pragma region MANGO_QUERY_API
	MANGO_QUERY_API bool ExecuteScriptQuery::match(std::string_view sql) const
	{
		return sql.starts_with("EXECUTE");
	}

	MANGO_QUERY_API void ExecuteScriptQuery::parse(std::string_view sql)
	{
		m_ScriptName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");
		sql.remove_suffix(1);

		auto args = StringUtils::splitAtChar(sql, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid execute script syntax");

		if (args[0] != "EXECUTE" || args[1] != "SCRIPT")
			throw InvalidArgumentException("Invalid execute script syntax");

		m_ScriptName = args[2];
	}

	MANGO_QUERY_API void ExecuteScriptQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!m_ScriptName.ends_with(".mango"))
			throw InvalidArgumentException("Only '.mango' script can be executed.");
	}

	MANGO_QUERY_API void ExecuteScriptQuery::execute(ref<MangoDB> dataBase)
	{
		throw FeatureNotImplementedException("Not yet...");
	}
#pragma endregion

}