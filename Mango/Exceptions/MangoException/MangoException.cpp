#include "standard_library.hpp"
#include "MangoException.hpp"

namespace Mango::Exceptions
{
	MangoException::MangoException(std::string&& what)
		: m_What(std::move(what))
	{}

	MangoException::MangoException(std::initializer_list<std::string_view> strings)
	{
		std::size_t length = 0;
		for (const auto& str : strings)
			length += str.length();

		m_What.reserve(length);

		for (const auto& str : strings)
			m_What.append(str);
	}

	std::string_view MangoException::what() const
	{
		return m_What;
	}

}