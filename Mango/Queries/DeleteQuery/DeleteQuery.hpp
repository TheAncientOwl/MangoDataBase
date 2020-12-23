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
	private: /// Fields
		std::string m_TableName;

	private: /// API
		bool QUERY_API match(std::string_view sql) const override;
		void QUERY_API parse(std::string_view sql) override;
		void QUERY_API validate(const_ref<MangoDB> dataBase) override;
		void QUERY_API execute(ref<MangoDB> dataBase) override;

	public: /// Constructors
		DeleteQuery() = default;
		DeleteQuery(const DeleteQuery&) = default;
		DeleteQuery(DeleteQuery&&) noexcept = default;
		DeleteQuery& operator=(const DeleteQuery&) = default;
		DeleteQuery& operator=(DeleteQuery&&) noexcept = default;
		~DeleteQuery() = default;
	};
}