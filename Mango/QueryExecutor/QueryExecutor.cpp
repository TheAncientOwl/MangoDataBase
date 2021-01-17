#include "standard_library.hpp"
#include "QueryExecutor.hpp"

using namespace Mango::Implementation::Queries;

#include "../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango
{
	void QueryExecutor::format(ref<std::string> sql) const
	{
		/// DELETE FROM table_name;
		/// DISPLAY */TABLE_NAME;
		/// DROP TABLE table_name;
		/// CREATE TABLE table_name (col1 dataType, col2 dataType, ...);
		/// SELECT */[col1, col2, ...] FROM table_name;
		/// UPDATE table_name; 
		/// TRUNCATE TABLE table_name;

		/// INSERT INTO table_name [col1, col2, ...] VALUES (val1, val2, ...);
		/// SAVE file_name.csv;
		/// IMPORT table_name file_name.csv;

		if (sql.size() > 5)
		{
			auto endOfCommand = std::begin(sql) + 6;
			std::transform(std::begin(sql), endOfCommand, std::begin(sql), ::toupper);

			if (sql.starts_with("INSERT"))
			{
				std::transform(endOfCommand, std::find(endOfCommand, std::end(sql), '('), std::begin(sql) + 6, ::toupper);
			}
			else if (sql.starts_with("IMPORT"))
			{
				auto space = std::find(std::begin(sql), std::end(sql), ' ');
				if (space != std::end(sql))
				{
					space = std::find(std::next(space), std::end(sql), ' ');
					std::transform(std::begin(sql) + 6, space, std::begin(sql) + 6, ::toupper);
				}
			}
			else if (!sql.starts_with("SAVE"))
			{
				std::transform(endOfCommand, std::end(sql), endOfCommand, ::toupper);
			}
		}
		else std::transform(std::cbegin(sql), std::cend(sql), std::begin(sql), ::toupper);
	}

	void QueryExecutor::execute(std::string&& sql, ref<MangoDB> dataBase)
	{
		format(sql);

		for (auto& query : s_Queries)
			if (query->match(sql))
			{
				query->parse(sql);
				query->validate(dataBase);
				query->execute(dataBase);

				return;
			}

		throw InvalidSyntaxException("Unknown command");
	}

	const std::array<std::unique_ptr<IQuery>, 10> QueryExecutor::s_Queries{
		std::make_unique<CreateTableQuery>(),
		std::make_unique<DropTableQuery>(),
		std::make_unique<TruncateTableQuery>(),
		std::make_unique<DescQuery>(),
		std::make_unique<InsertIntoQuery>(),
		std::make_unique<SelectQuery>(),
		std::make_unique<DeleteQuery>(),
		std::make_unique<UpdateQuery>(),
		std::make_unique<SaveDataQuery>(),
		std::make_unique<ImportDataQuery>()
	};
}