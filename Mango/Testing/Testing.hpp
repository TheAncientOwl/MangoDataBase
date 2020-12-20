#pragma once
#include "../MangoAPI.hpp"

namespace test
{
	using namespace Mango;

	std::string toUpper(std::string str);

	std::ostream& operator<<(std::ostream& out, const Column& col);
	void columnSerializeDeserialize();

	std::ostream& operator<<(std::ostream& out, const Table& table);
	void tableSerializeDeserialize();

	std::ostream& operator<<(std::ostream& out, const MangoDB& mango);
	void mangoSerializeDeserialize();
	
}