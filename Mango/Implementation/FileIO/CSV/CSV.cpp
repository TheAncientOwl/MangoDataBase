#include "standard_library.hpp"
#include "CSV.hpp"

namespace Mango::Implementation::FileIO::CSV
{
	void write(std::ofstream& csv, const_ref<Row> row)
	{
		auto& dataTypes = row.m_Config->m_DataTypes;

		for (int index = 0, size = static_cast<int>(dataTypes.size()); index < size; ++index)
		{
			switch (dataTypes[index])
			{
				case DataType::Value::INT:
				{
					csv << row.getInt(index);
					csv << (index != size - 1 ? "," : "");
					break;
				}
				case DataType::Value::FLOAT:
				{
					csv << row.getFloat(index);
					csv << (index != size - 1 ? "," : "");
					break;
				}
				case DataType::Value::STRING:
				{
					csv << row.getString(index);
					csv << (index != size - 1 ? "," : "");
					break;
				}
			}
		}

		csv << '\n';
	}

	void write(std::ofstream& csv, std::string_view val, char sep)
	{
		csv << val << sep;
	}

	std::ifstream& read(std::ifstream& csv, ref<Row> row)
	{



		return csv;
	}
}