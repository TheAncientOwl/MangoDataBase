#include "standard_library.hpp"
#include "DisplayQuery.hpp"

using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries
{
#pragma region MANGO_QUERY_INTERFACE
	MANGO_QUERY_INTERFACE bool DisplayQuery::match(std::string_view sql) const
	{
		return sql.starts_with("DISPLAY");
	}

	MANGO_QUERY_INTERFACE void DisplayQuery::parse(std::string_view sql)
	{
		m_TableName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = splitAtChar(sql, ' ');

		if (args.size() != 2)
			throw InvalidSyntaxException("Invalid display query syntax");

		if (args[0] != "DISPLAY")
			throw InvalidArgumentException({ "Unknwon sequence \"", args[0], "\"" });

		m_TableName = args[1];
	}

	MANGO_QUERY_INTERFACE void DisplayQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (m_TableName != "*")
			if (!dataBase.getTable(m_TableName))
				throw TableNotFoundException("Table does not exist", std::move(m_TableName));
	}

	MANGO_QUERY_INTERFACE void DisplayQuery::execute(ref<MangoDB> dataBase)
	{
		if (m_TableName == "*")
			std::cout << dataBase << '\n';
		else
			std::cout << *dataBase.getTable(m_TableName) << '\n';
	}
#pragma endregion
}