#pragma once
#include "../MangoExceptionType/MangoExceptionType.hpp"

namespace Mango::Exceptions
{
	class MangoException
	{
	private:
		std::string m_What;

	public:
		explicit MangoException(std::string&& what);

		explicit MangoException(std::initializer_list<std::string_view> strings);

		MangoException(const MangoException&) = delete;
		MangoException& operator=(const MangoException&) = delete;

		MangoException(MangoException&& other) noexcept = default;
		MangoException& operator=(MangoException&& other) noexcept = default;

		std::string_view what() const;

		virtual MangoExceptionType type() const = 0;

		virtual ~MangoException() = default;
	};
}