#pragma once
namespace Mango::Queries
{
	class AbstractQuery;
	class CreateTableQuery;
	class TruncateTableQuery;
	class DropTableQuery;
	class DisplayQuery;
	class InsertIntoQuery;
	class SelectQuery;
	class DeleteQuery;
	class UpdateQuery;
	namespace CommandLineAdapter
	{
		class SelectQueryCLI;
		class DeleteQueryCLI;
		class WhereCondition;
	}
}