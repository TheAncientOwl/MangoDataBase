#pragma once
#include "../MangoException/MangoException.hpp"

namespace Mango::Exceptions
{
	class NanException : public MangoException
	{
	public:
		explicit NanException(std::string&& what);

		explicit NanException(std::initializer_list<std::string_view> strings);

		MangoExceptionType type() const override;
	};
}