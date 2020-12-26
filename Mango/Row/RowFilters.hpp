#pragma once
#include "../Queries/Queries.fwd.hpp"
#include "RowFilters.fwd.hpp"

#include "../Row/Row.hpp"
#include "../Exceptions/InvalidArgumentException/InvalidArgumentException.hpp"
#include "../MangoDB/MangoDB.hpp"

namespace Mango
{
	class MangoDummyValues
	{
	private:
		int m_Index = 0;
		int m_Int = 0;
		float m_Float = 0;
		std::string_view m_String = "";

	public:
		static ref<MangoDummyValues> Instance();

		int getRowInt(const_ref<Row> row);
		float getRowFloat(const_ref<Row> row);
		std::string_view getRowString(const_ref<Row> row);

		int getInt();
		float getFloat();
		std::string_view getString();

	private:
		MangoDummyValues() = default;

	public:
		MangoDummyValues(const MangoDummyValues&) = delete;
		MangoDummyValues(MangoDummyValues&&) noexcept = delete;
		MangoDummyValues& operator=(const MangoDummyValues&) = delete;
		MangoDummyValues& operator=(MangoDummyValues&&) noexcept = delete;
		~MangoDummyValues() = default;

	private:
		friend struct Mango::Queries::CommandLineAdapter::WhereCondition;
	};

	class MangoClauseGuard
	{
	private:
		ref<MangoDB> m_DataBase;

	public:
		MangoClauseGuard(ref<MangoDB> dataBase);
		MangoClauseGuard(const MangoClauseGuard&) = delete;
		MangoClauseGuard(MangoClauseGuard&&) noexcept = delete;
		MangoClauseGuard& operator=(const MangoClauseGuard&) = delete;
		MangoClauseGuard& operator=(MangoClauseGuard&&) noexcept = delete;
		~MangoClauseGuard();
	};

	///Reference for Map template: https://www.youtube.com/watch?v=INn3xa4pMfg&t=860s
	template<typename Key, typename Value, size_t Size>
	struct Map
	{
		std::array<std::pair<Key, Value>, Size> data;

		[[nodiscard]] constexpr Value at(const Key& key) const
		{
			const auto it = std::find_if(std::begin(data), std::cend(data),
													[&key](const auto& v) { return v.first == key; });
			if (it != std::end(data))
				return it->second;
			else
				throw Exceptions::InvalidArgumentException("Unknown value");
		}
	};

	namespace RowFilters
	{
		using namespace std::literals::string_view_literals;
		namespace Int
		{
			static constexpr std::array<std::pair<std::string_view, WhereClause>, 6> whereClauses
			{ {
				{"="sv, &equals},
				{"!="sv, &notEquals},
				{"<"sv, &less},
				{">"sv, &greater},
				{"<="sv, &lessOrEquals},
				{">="sv, &greaterOrEquals}
			} };
		}

		namespace Float
		{
			static constexpr std::array<std::pair<std::string_view, WhereClause>, 6> whereClauses
			{ {
				{"="sv, &equals},
				{"!="sv, &notEquals},
				{"<"sv, &less},
				{">"sv, &greater},
				{"<="sv, &lessOrEquals},
				{">="sv, &greaterOrEquals}
			} };
		}

		namespace String
		{
			static constexpr std::array<std::pair<std::string_view, WhereClause>, 6> whereClauses
			{ {
				{"="sv, &equals},
				{"!="sv, &notEquals},
				{"<"sv, &less},
				{">"sv, &greater},
				{"<="sv, &lessOrEquals},
				{">="sv, &greaterOrEquals}
			} };
		}
	}
}