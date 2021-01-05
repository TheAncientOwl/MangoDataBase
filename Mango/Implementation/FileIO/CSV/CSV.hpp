#pragma once
#include "../../Row/Row.hpp"

namespace Mango::Implementation::FileIO::CSV
{
	std::ofstream& write(std::ofstream& csv, const_ref<Row> row);
	std::ifstream& read(std::ifstream& csv, ref<Row> row);
}