#include "standard_library.hpp"
#include "Statement.hpp"
#include "../../../Exceptions/InvalidSyntaxException/InvalidSyntaxException.hpp"

namespace Mango::Implementation::Queries
{
	Statement::Statement(const_ref<iterator> begin, const_ref<iterator> end, std::string_view beginChar, std::string_view endChar)
		: open(begin), closed(end), openChar(beginChar), closedChar(endChar)
	{
		assert(beginChar.size() == 1);
		assert(endChar.size() == 1);
	}

	void Statement::checkValidOrder(const_ref<iterator> defaultIt) const
	{
		using Mango::Exceptions::InvalidSyntaxException;
		if (open == defaultIt)
			throw InvalidSyntaxException({ "Missing '", openChar, "'" });

		if (closed == defaultIt)
			throw InvalidSyntaxException({ "Missing '", closedChar, "'" });

		if (closed < open)
			throw InvalidSyntaxException({ "Syntax error, found '", closedChar, "' before '", openChar, "'" });

	}
}