#pragma once
#include "../MangoException/MangoException.hpp"

namespace Mango::Exceptions
{
	class InvalidSyntaxException final : public MangoException
	{
	public:
		explicit InvalidSyntaxException(std::string&& what);

		explicit InvalidSyntaxException(std::initializer_list<std::string_view> strings);

		MangoExceptionType type() const override;
	};
}