#pragma once
#include "../../../MangoAPI.fwd.hpp"

#define MANGO_ESCAPE_CHAR '/'

#include "../Statement/Statement.hpp"
#include "../../../MangoDB/MangoDB.hpp"

namespace Mango::Implementation::Queries
{
	class AbstractQuery
	{
	private:
		static bool s_InvalidIdentifierChars[257];

	public:
		MANGO_PUBLIC_API static std::vector<std::string_view> splitAtChar(std::string_view str, char c);
		MANGO_PUBLIC_API static std::vector<std::string_view> splitAtCharWithEscape(std::string_view str, char c);
		MANGO_PUBLIC_API static void splitInCleanStringsAt(std::string_view str, char c, ref<std::vector<std::string>> out);
		MANGO_PUBLIC_API static bool isValidIdentifier(std::string_view identifier);
		MANGO_PUBLIC_API static std::string_view trimWhiteSpaces(std::string_view str);
		MANGO_PUBLIC_API static std::string cleanString(std::string_view str, char escapedChar);
		MANGO_PUBLIC_API static void removeEscapeChar(ref<std::string> str, char escapedChar);

	private:
		/// <summary>
		/// Checks if given sql may be this type of query.
		/// </summary>
		/// <param name="sql">-> query in uppercase.</param>
		MANGO_QUERY_API [[nodiscard]] virtual bool match(std::string_view sql) const = 0;

		/// <summary>
		/// Parse given sql and extracts desired query parts into atributes.
		/// [!] Use AbstractQuery::match before this.
		/// </summary>
		/// <param name="sql">-> query in uppercase.</param>
		MANGO_QUERY_API virtual void parse(std::string_view sql) = 0;

		/// <summary>
		/// Validate query parts extracted with AbstractQuery::parse.
		/// [!] Use AbstractQuery::parse before this.
		/// </summary>
		MANGO_QUERY_API virtual void validate(const_ref<MangoDB> dataBase) = 0;

		/// <summary>
		/// Execute query on data base.
		/// [!] Use AbstractQuery::validate before this.
		/// </summary>
		MANGO_QUERY_API virtual void execute(ref<MangoDB> dataBase) = 0;

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
		friend class Mango::CommandLineInterface;
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