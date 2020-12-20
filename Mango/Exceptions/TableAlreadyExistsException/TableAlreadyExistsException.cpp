#include "standard_library.hpp"
#include "TableAlreadyExistsException.hpp"

namespace Mango::Exceptions
{
	TableAlreadyExistsException::TableAlreadyExistsException(std::string&& what, std::string&& tableName)
		: MangoException(std::forward<std::string>(what)), m_TableName(std::forward<std::string>(tableName))
	{
	}

	TableAlreadyExistsException::TableAlreadyExistsException(std::initializer_list<std::string_view> strings, std::string&& tableName)
		: MangoException(std::forward<std::initializer_list<std::string_view>>(strings)), m_TableName(std::forward<std::string>(tableName))
	{
	}

	MangoExceptionType TableAlreadyExistsException::type() const
	{
		return MangoExceptionType::TABLE_ALREADY_EXISTS;
	}

	std::string_view TableAlreadyExistsException::tableName() const
	{
		return m_TableName;
	}
}