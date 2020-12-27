#pragma once
#include "../../UpdateQuery/UpdateQuery.hpp"
#include "../WhereCondition/WhereCondition.hpp"
#define MANGO_UPDATE_CLI_SYNTAX "UPDATE table_name SET (col = val, ...) WHERE <col = val>;"

namespace Mango::Implementation::Queries::CommandLineAdapter
{
	/// <summary>
	/// UPDATE table_name SET (col = val, ...) WHERE <col = val>;
	/// </summary>
	class UpdateQueryCLI final : public UpdateQuery
	{
	private:
		WhereCondition m_WhereCondition;

	private:
		/// <summary>
		/// () before <>
		/// </summary>
		MANGO_PRIVATE_API void checkStatementsOrder(Statement setClause, Statement condition, Statement::iterator defaultIt) const;
		
		/// <summary>
		/// (...) WHERE <...>;
		/// </summary>
		MANGO_PRIVATE_API void checkResidualParts(Statement setClause, Statement condition, 
											      Statement::iterator defaultIt, Statement::iterator stringEnd) const;

		/// <summary>
		/// UPDATE table_name SET
		/// </summary>
		MANGO_PRIVATE_API void parseTableName(std::string_view firstPart);

	private:
		MANGO_QUERY_INTERFACE void parse(std::string_view sql) override;
		MANGO_QUERY_INTERFACE void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_INTERFACE void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API UpdateQueryCLI() = default;
		MANGO_PUBLIC_API UpdateQueryCLI(const UpdateQueryCLI&) = default;
		MANGO_PUBLIC_API UpdateQueryCLI(UpdateQueryCLI&&) noexcept = default;
		MANGO_PUBLIC_API UpdateQueryCLI& operator=(const UpdateQueryCLI&) = default;
		MANGO_PUBLIC_API UpdateQueryCLI& operator=(UpdateQueryCLI&&) noexcept = default;
		MANGO_PUBLIC_API ~UpdateQueryCLI() = default;
	};
}