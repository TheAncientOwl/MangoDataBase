#include "standard_library.hpp"
#include "QueryExecutor.hpp"

namespace Mango
{
	void QueryExecutor::execute(std::string&& sql, ref<MangoDB> dataBase)
	{
		std::transform(std::begin(sql), std::end(sql), std::begin(sql), ::toupper);

		for (auto& query : s_Queries)
			if (query->match(sql))
			{
				query->parse(sql);
				query->validate(dataBase);
				query->execute(dataBase);

				return;
			}

		throw Mango::Exceptions::InvalidSyntaxException("Unknown command");
	}

	const std::array<std::unique_ptr<AbstractQuery>, 1> QueryExecutor::s_Queries{
		std::make_unique<CreateTableQuery>()
	};
}