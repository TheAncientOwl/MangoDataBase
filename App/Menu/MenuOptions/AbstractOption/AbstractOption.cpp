#include "standard_library.hpp"
#include "AbstractOption.hpp"

namespace App::MenuOptions
{
	AbstractMenuOption::AbstractMenuOption(std::string optionIdentifier)
		: m_OptionIdentifier(std::move(optionIdentifier))
	{
	}

	bool AbstractMenuOption::match(const_ref<std::string> optionIdentifier) const
	{
		return m_OptionIdentifier == optionIdentifier;
	}
}