#pragma once
#include "../../DeleteQuery/DeleteQuery.hpp"
#define MANGO_DELETE_CLI_SYNTAX "DELETE FROM table_name WHERE <col = val>;"

namespace Mango::Queries::CommandLineAdapter
{
	/// <summary>
	/// DELETE FROM table_name WHERE <col = val>;
	/// </summary>
	class DeleteQueryCLI final : public DeleteQuery
	{
	private:
		std::string m_WhereColumnName;
		std::string m_Operation;
		std::string m_Value;

		WhereClause m_WhereClause;

	private:
		MANGO_PRIVATE_API void parseCondition(std::string_view condition);

	private:
		MANGO_QUERY_INTERFACE void parse(std::string_view sql) override;
		MANGO_QUERY_INTERFACE void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_INTERFACE void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API DeleteQueryCLI() = default;
		MANGO_PUBLIC_API DeleteQueryCLI(const DeleteQueryCLI&) = default;
		MANGO_PUBLIC_API DeleteQueryCLI(DeleteQueryCLI&&) noexcept = default;
		MANGO_PUBLIC_API DeleteQueryCLI& operator=(const DeleteQueryCLI&) = default;
		MANGO_PUBLIC_API DeleteQueryCLI& operator=(DeleteQueryCLI&&) noexcept = default;
		MANGO_PUBLIC_API ~DeleteQueryCLI() = default;
	};
}