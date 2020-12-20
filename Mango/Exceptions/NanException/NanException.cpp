#include "standard_library.hpp"
#include "NanException.hpp"

namespace Mango::Exceptions
{
	NanException::NanException(std::string&& what)
		: MangoException(std::forward<std::string>(what))
	{
	}

	NanException::NanException(std::initializer_list<std::string_view> strings)
		: MangoException(std::forward<std::initializer_list<std::string_view>>(strings))
	{
	}

	MangoExceptionType NanException::type() const
	{
		return MangoExceptionType::NaN;
	}
}