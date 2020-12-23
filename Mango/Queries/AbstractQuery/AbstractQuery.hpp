#pragma once
namespace Mango
{
	class QueryExecutor;
}
#include "../../Exceptions/MangoExceptions.hpp"
#include "../Statement/Statement.hpp"
#include "../../MangoDB/MangoDB.hpp"

namespace Mango::Queries
{
	class AbstractQuery
	{
	private:
		static bool s_InvalidIdentifierChars[257];

	protected:
		MANGO_PROTECTED_API std::vector<std::string_view> splitAtAnyOf(std::string_view str, std::string_view delims) const;
		MANGO_PROTECTED_API std::vector<std::string_view> splitAtChar(std::string_view str, char c) const;
		MANGO_PROTECTED_API bool isValidIdentifier(std::string_view identifier) const;

	private:
		/// <summary>
		/// Checks if given sql may be this type of query.
		/// </summary>
		/// <param name="sql">-> query in uppercase.</param>
		MANGO_QUERY_INTERFACE [[nodiscard]] virtual bool match(std::string_view sql) const = 0;

		/// <summary>
		/// Parse given sql and extracts desired query parts into atributes.
		/// [!] Use AbstractQuery::match before this.
		/// </summary>
		/// <param name="sql">-> query in uppercase.</param>
		MANGO_QUERY_INTERFACE virtual void parse(std::string_view sql) = 0;

		/// <summary>
		/// Validate query parts extracted with AbstractQuery::parse.
		/// [!] Use AbstractQuery::parse before this.
		/// </summary>
		MANGO_QUERY_INTERFACE virtual void validate(const_ref<MangoDB> dataBase) = 0;

		/// <summary>
		/// Execute query on data base.
		/// [!] Use AbstractQuery::validate before this.
		/// </summary>
		MANGO_QUERY_INTERFACE virtual void execute(ref<MangoDB> dataBase) = 0;

	public:
		MANGO_PUBLIC_API AbstractQuery() = default;
		MANGO_PUBLIC_API AbstractQuery(const AbstractQuery&) = default;
		MANGO_PUBLIC_API AbstractQuery(AbstractQuery&&) noexcept = default;
		MANGO_PUBLIC_API AbstractQuery& operator=(const AbstractQuery&) = default;
		MANGO_PUBLIC_API AbstractQuery& operator=(AbstractQuery&&) noexcept = default;
		MANGO_PUBLIC_API virtual ~AbstractQuery() = default;

	private:
		friend class AbstractQueryStaticInitializer;
		friend class Mango::QueryExecutor;
	};

	class AbstractQueryStaticInitializer
	{
	public:
		MANGO_PUBLIC_API AbstractQueryStaticInitializer();
		MANGO_PUBLIC_API AbstractQueryStaticInitializer(const AbstractQueryStaticInitializer&) = delete;
		MANGO_PUBLIC_API AbstractQueryStaticInitializer(AbstractQueryStaticInitializer&&) noexcept = delete;
		MANGO_PUBLIC_API AbstractQueryStaticInitializer& operator=(const AbstractQueryStaticInitializer&) = delete;
		MANGO_PUBLIC_API AbstractQueryStaticInitializer& operator=(AbstractQueryStaticInitializer&&) noexcept = delete;
		MANGO_PUBLIC_API ~AbstractQueryStaticInitializer() = default;
	};
}