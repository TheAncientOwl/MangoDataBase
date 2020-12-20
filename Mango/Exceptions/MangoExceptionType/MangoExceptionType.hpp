#pragma once

namespace Mango::Exceptions
{
	enum class MangoExceptionType : uint8_t
	{
		INVALID_SYNTAX,
		INVALID_ARGUMENT,
		NaN,
		TABLE_ALREADY_EXISTS,
		TABLE_NOT_FOUND,
		FEATURE_NOT_IMPLEMENTED
	};
}