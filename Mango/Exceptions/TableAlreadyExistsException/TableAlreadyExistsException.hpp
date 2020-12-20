#pragma once
#include "../MangoException/MangoException.hpp"

namespace Mango::Exceptions
{
	class TableAlreadyExistsException : public MangoException
	{
	private:
		std::string m_TableName;

	public:
		explicit TableAlreadyExistsException(std::string&& what, std::string&& tableName);

		explicit TableAlreadyExistsException(std::initializer_list<std::string_view> strings, std::string&& tableName);

		MangoExceptionType type() const override;

		std::string_view tableName() const;
	};
}