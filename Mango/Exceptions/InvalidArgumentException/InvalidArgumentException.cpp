#include "standard_library.hpp"
#include "InvalidArgumentException.hpp"

namespace Mango::Exceptions
{
	InvalidArgumentException::InvalidArgumentException(std::string&& what)
		: MangoException(std::forward<std::string>(what))
	{
	}

	InvalidArgumentException::InvalidArgumentException(std::initializer_list<std::string_view> strings)
		: MangoException(std::forward<std::initializer_list<std::string_view>>(strings))
	{
	}

	MangoExceptionType InvalidArgumentException::type() const
	{
		return MangoExceptionType::INVALID_ARGUMENT;
	}
}