#pragma once
#include "../Row/Row.fwd.hpp"

namespace Mango::Implementation::Queries
{
	class IQuery;
	class CreateTableQuery;
	class TruncateTableQuery;
	class DropTableQuery;
	class DescQuery;
	class InsertIntoQuery;
	class SelectQuery;
	class DeleteQuery;
	class UpdateQuery;
	namespace CommandLineAdapter
	{
		class SelectQueryCLI;
		class DeleteQueryCLI;
		class UpdateQueryCLI;
		class WhereCondition;
		class SetClauseData;
		void setClauseCLI(ref<Row> row);
	}
	class SaveDataQuery;
	class ImportDataQuery;
	class ExecuteScriptQuery;
	class DisplayQuery;
}