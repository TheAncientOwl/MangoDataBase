#include "standard_library.hpp"
#include "DeleteQuery.hpp"

namespace Mango::Queries
{
	using namespace Mango::Exceptions;

	bool DeleteQuery::match(std::string_view sql) const
	{
		return sql.starts_with("DELETE");
	}

	void DeleteQuery::parse(std::string_view sql)
	{
		m_TableName.clear();

		if (sql.back() != ';')
			throw InvalidSyntaxException("Missing ';'");

		sql.remove_suffix(1);

		auto args = splitAtChar(sql, ' ');

		if (args.size() != 3)
			throw InvalidSyntaxException("Invalid delete query syntax");

		if (args[0] != "DELETE" || args[1] != "FROM")
			throw InvalidArgumentException("Check missing \"DELETE\" nor \"FROM\" keywords");

		m_TableName = args[2];
	}

	void DeleteQuery::validate(const_ref<MangoDB> dataBase)
	{
		if (!dataBase.getTable(m_TableName))
			throw TableNotFoundException("Table does not exists", std::move(m_TableName));
	}

	void DeleteQuery::execute(ref<MangoDB> dataBase)
	{
		auto table = dataBase.getTable(m_TableName);

		auto tableIterator = table->makeIterator();

		std::filesystem::path dummyFilePath = table->getDataFilePath();
		dummyFilePath.replace_filename("dummy");
		std::fstream file(dummyFilePath, std::ios::out | std::ios::trunc | std::ios::binary);

		while (tableIterator.advance())
		{
			auto& row = tableIterator.row();
			if (dataBase.m_DeleteFilter(row))
				serializePOD(file, row.data(), row.size());
		}

		file.close();
		tableIterator.releaseFile();

		std::filesystem::remove(table->getDataFilePath());
		std::filesystem::rename(dummyFilePath, table->getDataFilePath());
	}
}