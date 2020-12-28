#include "standard_library.hpp"
#include "AbstractQuery.hpp"

namespace Mango::Implementation::Queries
{
#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API std::vector<std::string_view> AbstractQuery::splitAtChar(std::string_view str, char c)
	{
		std::vector<std::string_view> out;

		auto first = std::cbegin(str);
		auto last = std::find(first, std::cend(str), c);

		while (last != std::cend(str))
		{
			if (first != last)
				out.emplace_back(first._Unwrapped(), std::distance(first, last));

			first = std::next(last);
			last = std::find(first, std::cend(str), c);
		}

		if (first != last)
		{
			std::string_view s(first._Unwrapped(), std::distance(first, last));
			if (!s.empty())
				out.push_back(std::move(s));
		}

		return out;
	}

	MANGO_PUBLIC_API bool AbstractQuery::isValidIdentifier(std::string_view identifier)
	{
		if (identifier.empty())
			return false;

		if ('0' <= identifier.front() && identifier.front() <= '9')
			return false;

		for (auto it = std::begin(identifier), end = std::end(identifier); it != end; ++it)
			if (s_InvalidIdentifierChars[*it])
				return false;

		return true;
	}

	MANGO_PUBLIC_API std::string_view AbstractQuery::trimWhiteSpaces(std::string_view str)
	{
		while (!str.empty() && str.front() == ' ')
			str.remove_prefix(1);
		while (!str.empty() && str.back() == ' ')
			str.remove_suffix(1);
		return str;
	}
#pragma endregion

#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API AbstractQueryStaticInitializer::AbstractQueryStaticInitializer()
	{
		for (bool& b : AbstractQuery::s_InvalidIdentifierChars)
			b = false;

		std::string_view s("`~!@#$%^&*()-+={}[]|\\;:'\",.<>/?");
		for (auto c : s)
			AbstractQuery::s_InvalidIdentifierChars[c] = true;
	}
#pragma endregion

	bool AbstractQuery::s_InvalidIdentifierChars[257];

	static_block
	{
		AbstractQueryStaticInitializer _;
	};
}