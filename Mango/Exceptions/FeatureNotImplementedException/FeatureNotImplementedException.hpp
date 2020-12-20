#pragma once
#include "../MangoException/MangoException.hpp"

namespace Mango::Exceptions
{
	class FeatureNotImplementedException : public MangoException
	{
	public:
		explicit FeatureNotImplementedException(std::string&& what);

		explicit FeatureNotImplementedException(std::initializer_list<std::string_view> strings);

		MangoExceptionType type() const override;
	};
}