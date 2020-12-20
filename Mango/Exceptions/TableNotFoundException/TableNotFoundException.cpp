#include "standard_library.hpp"
#include "TableNotFoundException.hpp"

namespace Mango::Exceptions
{
	TableNotFoundException::TableNotFoundException(std::string&& what, std::string&& tableName)
		: MangoException(std::forward<std::string>(what)), m_TableName(std::forward<std::string>(tableName))
	{
	}

	TableNotFoundException::TableNotFoundException(std::initializer_list<std::string_view> strings, std::string&& tableName)
		: MangoException(std::forward<std::initializer_list<std::string_view>>(strings)), m_TableName(std::forward<std::string>(tableName))
	{
	}

	MangoExceptionType TableNotFoundException::type() const
	{
		return MangoExceptionType::TABLE_NOT_FOUND;
	}

	std::string_view TableNotFoundException::tableName() const
	{
		return m_TableName;
	}
}