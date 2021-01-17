#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_IMPORT_DATA_SYNTAX "IMPORT table_name file_name.csv;"

namespace Mango::Implementation::Queries
{
	/// <summary>
	/// IMPORT table_name file_name.csv;
	/// Imported data must match full table configuration.
	/// Imported strings must not contain ','
	/// </summary>
	class ImportDataQuery final : public IQuery
	{
	private:
		std::string m_TableName;
		std::string m_FileName;

	private:
		MANGO_QUERY_API bool match(std::string_view sql) const override;
		MANGO_QUERY_API void parse(std::string_view sql) override;
		MANGO_QUERY_API void validate(const_ref<MangoDB> dataBase) override;
		MANGO_QUERY_API void execute(ref<MangoDB> dataBase) override;

	public:
		MANGO_PUBLIC_API ImportDataQuery() = default;
		MANGO_PUBLIC_API ImportDataQuery(const ImportDataQuery&) = default;
		MANGO_PUBLIC_API ImportDataQuery(ImportDataQuery&&) noexcept = default;
		MANGO_PUBLIC_API ImportDataQuery& operator=(const ImportDataQuery&) = default;
		MANGO_PUBLIC_API ImportDataQuery& operator=(ImportDataQuery&&) noexcept = default;
		MANGO_PUBLIC_API ~ImportDataQuery() = default;
	};
}