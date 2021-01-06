#pragma once

namespace Mango::Implementation
{
	class Row;
	class RowConfiguration;
}

namespace Mango::Implementation::FileIO::CSV
{
	void write(std::ofstream& csv, const_ref<Row> row);
	void write(std::ofstream& csv, std::string_view val, char sep = ',');
	std::ifstream& read(std::ifstream& csv, ref<Row> row);
}