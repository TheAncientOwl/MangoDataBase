#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_SAVE_DATA_SYNTAX "SAVE file_name.csv;"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// SAVE file_name.csv;
	/// </summary>
	class SaveDataQuery final : public AbstractQuery
	{
	private:
		std::string m_FileName;

	private:
		MANGO_QUERY_API bool match(std::string_view sql) const override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API SaveDataQuery() = default;
		MANGO_PUBLIC_API SaveDataQuery(const SaveDataQuery&) = default;
		MANGO_PUBLIC_API SaveDataQuery(SaveDataQuery&&) noexcept = default;
		MANGO_PUBLIC_API SaveDataQuery& operator=(const SaveDataQuery&) = default;
		MANGO_PUBLIC_API SaveDataQuery& operator=(SaveDataQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~SaveDataQuery() = default;
	};
}