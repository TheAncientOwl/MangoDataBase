#include "standard_library.hpp"
#include "DisplayQuery.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

#include "../../../ConsoleColorOutput/ConsoleColorOutput.hpp"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

namespace Mango::Implementation::Queries
{
#pragma region MANGO_API
	MANGO_API int DisplayQuery::digitsNo(int x) const
	{
		int n = 0;
		while (x > 0)
		{
			n++;
			x /= 10;
		}
		return n;
	}

	MANGO_API int DisplayQuery::digitsNo(float x) const
	{
		while (x != (int)x)
			x *= 10;

		return digitsNo(int(x)) + 1;
	}
#pragma endregion


#pragma region MANGO_QUERY_API
	bool DisplayQuery::match(std::string_view sql) const
	{
		return sql.starts_with("DISPLAY");
	}

	MANGO_QUERY_API void DisplayQuery::parse(std::string_view sql)
	{
		if (sql.back() != ';')
			throw InvalidArgumentException("Misssing ';'");
	}

	MANGO_QUERY_API void DisplayQuery::validate(const_ref<MangoDB> dataBase)
	{
	}

	MANGO_QUERY_API void DisplayQuery::execute(ref<MangoDB> dataBase)
	{
		const auto& rows = dataBase.lastResult();
		if (rows.empty())
			std::cout << ccolor::dark_gray << ">> " << ccolor::green << "No data found" << std::endl;
		else
		{
			const auto& columns = dataBase.lastColumns();
			const int COLUMNS_NUMBER = static_cast<int>(columns.size());

			std::vector<int> maxColumnWidths;
			maxColumnWidths.reserve(COLUMNS_NUMBER);

			for (const auto& rowName : columns)
				maxColumnWidths.push_back(static_cast<int>(rowName.size()));

			/// calculate max widths.
			using Mango::Implementation::DataType;
			for (const auto& row : rows)
				for (int i = 0; i < COLUMNS_NUMBER; ++i)
				{
					int width = 0;
					switch (row.m_Config->dataTypeAt(i))
					{
						case DataType::Value::INT:    width = digitsNo(row.getInt(i));				     break;
						case DataType::Value::FLOAT:  width = digitsNo(row.getFloat(i));				 break;
						case DataType::Value::STRING: width = static_cast<int>(row.getString(i).size()); break;
					}

					maxColumnWidths[i] = MAX(width, maxColumnWidths[i]);
				}

			/// print column names.
			std::cout << ccolor::dark_gray << "| ";
			for (int i = 0; i < COLUMNS_NUMBER; ++i)
			{
				std::cout.width(maxColumnWidths[i]);
				std::cout << ccolor::dark_red << columns[i] << ccolor::dark_gray << " | ";
			}
			std::cout << '\n';

			/// print separator line.
			std::cout << ccolor::dark_gray << "|-";
			std::cout.fill('-');
			for (int i = 0; i < COLUMNS_NUMBER; ++i)
			{
				std::cout.width(maxColumnWidths[i]);
				std::cout << "-";
				std::cout.width(1);
				std::cout << (i < COLUMNS_NUMBER - 1 ? "-|-" : "-|");
			}
			std::cout.fill(' ');
			std::cout << '\n';

			/// print rows.
			std::cout << std::fixed << std::setprecision(2);
			for (const auto& row : rows)
			{
				std::cout << ccolor::dark_gray << "| ";
				for (int i = 0; i < COLUMNS_NUMBER; ++i)
				{
					std::cout.width(maxColumnWidths[i]);
					std::cout << ccolor::green;
					switch (row.m_Config->dataTypeAt(i))
					{
						case DataType::Value::INT:    std::cout << row.getInt(i);    break;
						case DataType::Value::FLOAT:  std::cout << row.getFloat(i);  break;
						case DataType::Value::STRING: std::cout << row.getString(i); break;
					}
					std::cout << ccolor::dark_gray << " | ";
				}
				std::cout << '\n';
			}
			std::cout.width(1);
			std::cout << std::endl;
		}
	}
#pragma endregion

}