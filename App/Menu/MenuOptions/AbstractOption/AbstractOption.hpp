#pragma once

namespace App::MenuOptions
{
	class AbstractMenuOption
	{
	protected:
		std::string m_OptionIdentifier;

	public:
		bool match(const_ref<std::string> optionIdentifier) const;
		virtual void execute() const = 0;

	public:
		AbstractMenuOption(std::string optionIdentifier);
		AbstractMenuOption(const AbstractMenuOption&) = default;
		AbstractMenuOption(AbstractMenuOption&&) noexcept = default;
		AbstractMenuOption& operator=(const AbstractMenuOption&) = default;
		AbstractMenuOption& operator=(AbstractMenuOption&&) noexcept = default;
		virtual ~AbstractMenuOption() = default;
	};
}