#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_UPDATE_SYNTAX "UPDATE table_name;"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// UPDATE table_name;
	/// [!] Where clause can be specified through MagoDB::setWhereClause(WhereClause whereClause);
	/// [!] By default all values are updated.
	/// [!] Set clause can be specified through MangoDB::setSetClause(SetClause setClause);
	/// [!] By default nothing will be modified.
	/// </summary>
	class UpdateQuery : public IQuery
	{
	protected:
		std::string m_TableName;

	protected:
		MANGO_QUERY_API bool match(std::string_view sql) const final override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API UpdateQuery() = default;
		MANGO_PUBLIC_API UpdateQuery(const UpdateQuery&) = default;
		MANGO_PUBLIC_API UpdateQuery(UpdateQuery&&) noexcept = default;
		MANGO_PUBLIC_API UpdateQuery& operator=(const UpdateQuery&) = default;
		MANGO_PUBLIC_API UpdateQuery& operator=(UpdateQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~UpdateQuery() = default;
	};
}