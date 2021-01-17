#pragma once
#include "../../../MangoAPI.fwd.hpp"

#include "../../StringUtils/StringUtils.hpp"
#include "../Statement/Statement.hpp"
#include "../../../MangoDB/MangoDB.hpp"

namespace Mango::Implementation::Queries
{
	class IQuery
	{
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
		MANGO_PUBLIC_API IQuery() = default;
		MANGO_PUBLIC_API IQuery(const IQuery&) = default;
		MANGO_PUBLIC_API IQuery(IQuery&&) noexcept = default;
		MANGO_PUBLIC_API IQuery& operator=(const IQuery&) = default;
		MANGO_PUBLIC_API IQuery& operator=(IQuery&&) noexcept = default;
		MANGO_PUBLIC_API virtual ~IQuery() = default;

	private:
		friend class Mango::QueryExecutor;
		friend class Mango::CommandLineInterface;
	};
}