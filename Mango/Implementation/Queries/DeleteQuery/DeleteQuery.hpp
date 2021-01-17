#pragma once
#include "../../Row/RowFilters.hpp"
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_DELETE_SYNTAX "DELETE FROM table_name;"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// DELETE FROM table_name;
	/// [!] Where clause can be specified through MagoDB::setWhereClause(WhereClause whereClause);
	/// [!] By default all values are deleted.
	/// </summary>
	class DeleteQuery : public IQuery
	{
	protected:
		std::string m_TableName;

	protected:
		MANGO_QUERY_API bool match(std::string_view sql) const final override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API DeleteQuery() = default;
		MANGO_PUBLIC_API DeleteQuery(const DeleteQuery&) = default;
		MANGO_PUBLIC_API DeleteQuery(DeleteQuery&&) noexcept = default;
		MANGO_PUBLIC_API DeleteQuery& operator=(const DeleteQuery&) = default;
		MANGO_PUBLIC_API DeleteQuery& operator=(DeleteQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~DeleteQuery() = default;
	};
}