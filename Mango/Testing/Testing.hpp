#pragma once
#include "../MangoAPI.hpp"

namespace test
{
	using namespace Mango;

	std::string toUpper(std::string str);
	void columnSerializeDeserialize();
	void tableSerializeDeserialize();
	void mangoSerializeDeserialize();
	void testRow();
}