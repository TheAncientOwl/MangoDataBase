#include "standard_library.hpp"
#include "RowFilters.hpp"

namespace Mango
{
#pragma region MangoDummyValues
	ref<MangoDummyValues> Mango::MangoDummyValues::Instance()
	{
		static MangoDummyValues dummy;
		return dummy;
	}

	int MangoDummyValues::getRowInt(const_ref<Row> row)
	{
		return row.getInt(m_Index);
	}

	float MangoDummyValues::getRowFloat(const_ref<Row> row)
	{
		return row.getFloat(m_Index);
	}

	std::string_view MangoDummyValues::getRowString(const_ref<Row> row)
	{
		return row.getString(m_Index);
	}

	int MangoDummyValues::getInt()
	{
		return m_Int;
	}

	float MangoDummyValues::getFloat()
	{
		return m_Float;
	}

	std::string_view MangoDummyValues::getString()
	{
		return m_String;
	}
#pragma endregion

#pragma region MangoClauseGuard
	MangoClauseGuard::MangoClauseGuard(ref<MangoDB> dataBase) : m_DataBase(dataBase)
	{
	}

	MangoClauseGuard::~MangoClauseGuard()
	{
		m_DataBase.m_WhereClause = &RowFilters::allwaysTrue;
		m_DataBase.m_SetClause = &RowFilters::doNothing;
	}
#pragma endregion

#pragma region RowFilters
	namespace RowFilters
	{
		bool allwaysTrue(const_ref<Row> row)
		{
			return true;
		}

		bool allwaysFalse(const_ref<Row> row)
		{
			return false;
		}

		void doNothing(ref<Row> row)
		{
			//~NOTHING~ What would you have expected? xD
		}

		namespace Int
		{
			bool equals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowInt(row) == dummy.getInt();
			}

			bool notEquals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowInt(row) != dummy.getInt();
			}

			bool less(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowInt(row) < dummy.getInt();
			}

			bool greater(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowInt(row) > dummy.getInt();
			}

			bool lessOrEquals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowInt(row) <= dummy.getInt();
			}

			bool greaterOrEquals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowInt(row) >= dummy.getInt();
			}

			WhereClause GetWhereClause(const std::string_view sv)
			{
				static constexpr auto map = Map<std::string_view, WhereClause, whereClauses.size()>{ {whereClauses} };
				return map.at(sv);
			}
		}

		namespace Float
		{
			bool equals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowFloat(row) == dummy.getFloat();
			}

			bool notEquals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowFloat(row) != dummy.getFloat();
			}

			bool less(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowFloat(row) < dummy.getFloat();
			}

			bool greater(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowFloat(row) > dummy.getFloat();
			}

			bool lessOrEquals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowFloat(row) <= dummy.getFloat();
			}

			bool greaterOrEquals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowFloat(row) >= dummy.getFloat();
			}

			WhereClause GetWhereClause(const std::string_view sv)
			{
				static constexpr auto map = Map<std::string_view, WhereClause, whereClauses.size()>{ {whereClauses} };
				return map.at(sv);
			}
		}

		namespace String
		{
			bool equals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowString(row) == dummy.getString();
			}

			bool notEquals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowString(row) != dummy.getString();
			}

			bool less(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowString(row) < dummy.getString();
			}

			bool greater(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowString(row) > dummy.getString();
			}

			bool lessOrEquals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowString(row) <= dummy.getString();
			}

			bool greaterOrEquals(const_ref<Row> row)
			{
				ref<MangoDummyValues> dummy = MangoDummyValues::Instance();
				return dummy.getRowString(row) >= dummy.getString();
			}

			WhereClause GetWhereClause(const std::string_view sv)
			{
				static constexpr auto map = Map<std::string_view, WhereClause, whereClauses.size()>{ {whereClauses} };
				return map.at(sv);
			}
		}
	}
#pragma endregion

}