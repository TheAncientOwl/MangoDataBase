#include "standard_library.hpp"
#include "AbstractQuery.hpp"

#include "../../StaticBlock/StaticBlock.hpp"

#include "../../../Exceptions/MangoExceptions.hpp"
using namespace Mango::Exceptions;

namespace Mango::Implementation::Queries
{
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

	MANGO_STATIC_BLOCK
	{
		AbstractQueryStaticInitializer _;
	};
}