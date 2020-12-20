#include "standard_library.hpp"
#include "InvalidSyntaxException.hpp"

namespace Mango::Exceptions
{
	InvalidSyntaxException::InvalidSyntaxException(std::string&& what)
		: MangoException(std::forward<std::string>(what))
	{
	}

	InvalidSyntaxException::InvalidSyntaxException(std::initializer_list<std::string_view> strings)
		: MangoException(std::forward<std::initializer_list<std::string_view>>(strings))
	{
	}

	MangoExceptionType InvalidSyntaxException::type() const
	{
		return MangoExceptionType::INVALID_SYNTAX;
	}
}