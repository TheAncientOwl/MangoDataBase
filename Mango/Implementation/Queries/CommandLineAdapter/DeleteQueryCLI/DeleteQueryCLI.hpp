#pragma once
#include "../../DeleteQuery/DeleteQuery.hpp"
#include "../WhereCondition/WhereCondition.hpp"
#define MANGO_DELETE_CLI_SYNTAX "DELETE FROM table_name WHERE <col = val>;"

namespace Mango::Implementation::Queries::CommandLineAdapter
{
	/// <summary>
	/// DELETE FROM table_name WHERE <col = val>;
	/// </summary>
	class DeleteQueryCLI final : public DeleteQuery
	{
	private:
		WhereCondition m_WhereCondition;

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