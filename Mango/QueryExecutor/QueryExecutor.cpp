#include "standard_library.hpp"
#include "QueryExecutor.hpp"

#include "../Exceptions/MangoExceptions.hpp"
using namespace Mango::Implementation::Queries;

namespace Mango
{
	void QueryExecutor::execute(std::string&& sql, ref<MangoDB> dataBase)
	{
		if (sql.size() > 5)
		{
			auto wannaBeEndOfInsert = std::cbegin(sql) + 6;
			std::transform(std::cbegin(sql), wannaBeEndOfInsert, std::begin(sql), ::toupper);

			auto upperEnd = std::cend(sql);
			if (sql.starts_with("INSERT"))
				upperEnd = std::find(wannaBeEndOfInsert, std::cend(sql), '(');

			std::transform(wannaBeEndOfInsert, upperEnd, std::begin(sql) + 6, ::toupper);
		}
		else std::transform(std::begin(sql), std::end(sql), std::begin(sql), ::toupper);

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

	const std::array<std::unique_ptr<AbstractQuery>, 8> QueryExecutor::s_Queries{
		std::make_unique<CreateTableQuery>(),
		std::make_unique<DropTableQuery>(),
		std::make_unique<TruncateTableQuery>(),
		std::make_unique<DisplayQuery>(),
		std::make_unique<InsertIntoQuery>(),
		std::make_unique<SelectQuery>(),
		std::make_unique<DeleteQuery>(),
		std::make_unique<UpdateQuery>()
	};
}