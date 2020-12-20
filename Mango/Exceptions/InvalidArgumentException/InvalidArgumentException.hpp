#pragma once
#include "../MangoException/MangoException.hpp"

namespace Mango::Exceptions
{
	class InvalidArgumentException : public MangoException
	{
	public:
		explicit InvalidArgumentException(std::string&& what);

		explicit InvalidArgumentException(std::initializer_list<std::string_view> strings);

		MangoExceptionType type() const override;
	};
}