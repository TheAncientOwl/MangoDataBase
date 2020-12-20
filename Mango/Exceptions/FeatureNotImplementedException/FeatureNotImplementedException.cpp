#include "standard_library.hpp"
#include "FeatureNotImplementedException.hpp"

namespace Mango::Exceptions
{
	FeatureNotImplementedException::FeatureNotImplementedException(std::string&& what)
		: MangoException(std::forward<std::string>(what))
	{
	}

	FeatureNotImplementedException::FeatureNotImplementedException(std::initializer_list<std::string_view> strings)
		: MangoException(std::forward<std::initializer_list<std::string_view>>(strings))
	{
	}

	MangoExceptionType FeatureNotImplementedException::type() const
	{
		return MangoExceptionType::FEATURE_NOT_IMPLEMENTED;
	}
}