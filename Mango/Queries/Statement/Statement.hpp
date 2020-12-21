#pragma once

namespace Mango::Queries
{
	struct Statement
	{
		using iterator = std::string_view::const_iterator;
		iterator open;
		iterator closed;
		std::string_view openChar;
		std::string_view closedChar;

		Statement(const_ref<iterator> begin, const_ref<iterator> end, std::string_view beginChar, std::string_view endChar);
		Statement() = default;

		Statement(const Statement&) = default;
		Statement& operator=(const Statement&) = default;

		Statement(Statement&&) noexcept = default;
		Statement& operator=(Statement&&) noexcept = default;

		~Statement() = default;

		void checkValidOrder(const_ref<iterator> defaultIt) const;
	};
}