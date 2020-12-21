#pragma once
namespace Mango
{
	class QueryExecutor;
}
#include "../../Exceptions/MangoExceptions.hpp"
#include "../../MangoDB/MangoDB.hpp"

#define QUERY_INTERFACE

namespace Mango::Queries
{
	class AbstractQuery
	{
	private: /// Fields
		static bool s_InvalidIdentifierChars[257];

	protected: /// API
		std::vector<std::string_view> PROTECTED_API splitAtAnyOf(std::string_view str, std::string_view delims) const;
		std::vector<std::string_view> PROTECTED_API splitAtChar(std::string_view str, char c) const;
		bool PROTECTED_API isValidIdentifier(std::string_view identifier) const;

	private: /// Interface
	/// <summary>
	/// Checks if given sql may be this type of query.
	/// </summary>
	/// <param name="sql">-> query in uppercase.</param>
		[[nodiscard]] virtual bool QUERY_INTERFACE match(std::string_view sql) const = 0;

		/// <summary>
		/// Parse given sql and extracts desired query parts into atributes.
		/// [!] Use AbstractQuery::match before this.
		/// </summary>
		/// <param name="sql">-> query in uppercase.</param>
		virtual void QUERY_INTERFACE parse(std::string_view sql) = 0;

		/// <summary>
		/// Validate query parts extracted with AbstractQuery::parse.
		/// [!] Use AbstractQuery::parse before this.
		/// </summary>
		virtual void QUERY_INTERFACE validate(const_ref<MangoDB> dataBase) = 0;

		/// <summary>
		/// Execute query on data base.
		/// [!] Use AbstractQuery::validate before this.
		/// </summary>
		virtual void QUERY_INTERFACE execute(ref<MangoDB> dataBase) = 0;

	public: /// Constructors
		AbstractQuery() = default;
		AbstractQuery(const AbstractQuery&) = default;
		AbstractQuery(AbstractQuery&&) noexcept = default;
		AbstractQuery& operator=(const AbstractQuery&) = default;
		AbstractQuery& operator=(AbstractQuery&&) noexcept = default;
		virtual ~AbstractQuery() = default;

	private: /// Friends
		friend class AbstractQueryStaticInitializer;
		friend class Mango::QueryExecutor;
	};

	class AbstractQueryStaticInitializer
	{
	public:
		AbstractQueryStaticInitializer();
		AbstractQueryStaticInitializer(const AbstractQueryStaticInitializer&) = delete;
		AbstractQueryStaticInitializer(AbstractQueryStaticInitializer&&) noexcept = delete;
		AbstractQueryStaticInitializer& operator=(const AbstractQueryStaticInitializer&) = delete;
		AbstractQueryStaticInitializer& operator=(AbstractQueryStaticInitializer&&) noexcept = delete;
		~AbstractQueryStaticInitializer() = default;
	};
}