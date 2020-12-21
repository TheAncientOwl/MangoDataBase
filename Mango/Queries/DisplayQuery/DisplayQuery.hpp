#pragma once
#include "../AbstractQuery/AbstractQuery.hpp"
#define MANGO_DISPLAY_SYNTAX "DISPLAY */TABLE_NAME"

namespace Mango::Queries
{
	/// <summary>
	/// DISPLAY */TABLE_NAME;
	/// </summary>
	class DisplayQuery final : public AbstractQuery
	{
	private: /// Fields
		std::string m_TableName;

	private: /// API
		bool QUERY_API match(std::string_view sql) const override;
		void QUERY_API parse(std::string_view sql) override;
		void QUERY_API validate(const_ref<MangoDB> dataBase) override;
		void QUERY_API execute(ref<MangoDB> dataBase) override;

	public: /// Constructors
		DisplayQuery() = default;
		DisplayQuery(const DisplayQuery&) = default;
		DisplayQuery(DisplayQuery&&) noexcept = default;
		DisplayQuery& operator=(const DisplayQuery&) = default;
		DisplayQuery& operator=(DisplayQuery&&) noexcept = default;
		~DisplayQuery() = default;
	};
}