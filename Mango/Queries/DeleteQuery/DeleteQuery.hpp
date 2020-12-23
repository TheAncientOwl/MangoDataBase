#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_DELETE_SYNTAX "DELETE FROM table_name;"

namespace Mango::Queries
{
	/// <summary>
	/// DELETE FROM table_name;
	/// [!] Delete filter can be specified through MagoDB::setDeleteFilter(RowFilter selectFilter);
	/// </summary>
	class DeleteQuery final : public AbstractQuery
	{
	private:
		std::string m_TableName;

	private:
		MANGO_QUERY_INTERFACE bool match(std::string_view sql) const override;
		MANGO_QUERY_INTERFACE void parse(std::string_view sql) override;
		MANGO_QUERY_INTERFACE void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_INTERFACE void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API DeleteQuery() = default;
		MANGO_PUBLIC_API DeleteQuery(const DeleteQuery&) = default;
		MANGO_PUBLIC_API DeleteQuery(DeleteQuery&&) noexcept = default;
		MANGO_PUBLIC_API DeleteQuery& operator=(const DeleteQuery&) = default;
		MANGO_PUBLIC_API DeleteQuery& operator=(DeleteQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~DeleteQuery() = default;
	};
}