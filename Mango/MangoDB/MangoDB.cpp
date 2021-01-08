#include "standard_library.hpp"
#include "MangoDB.hpp"

#include "../Implementation/FileIO/Binary/Binary.hpp"
using namespace Mango::Implementation::FileIO::Binary;

#include "../ConsoleColorOutput/ConsoleColorOutput.hpp"

#define MANGO_STREAM_OPERATOR_HEADER    ccolor::dark_gray << "|___________________________________[ "\
									 << ccolor::purple << "Tables"\
									 << ccolor::dark_gray << " ]____________________________________|"\

#define MANGO_CLI_SEPARATOR_LINE ccolor::dark_gray << "|_________________________________________________________________________________|"

namespace Mango
{
#pragma region MANGO_API
	MANGO_API std::filesystem::path MangoDB::getConfigFilePath() const
	{
		return m_DirectoryPath / "tables" MANGO_CONFIG_EXTENSION;
	}

	MANGO_API void MangoDB::addTable(std::unique_ptr<Implementation::Table> table)
	{
		table->createFiles();
		m_Tables.push_back(std::move(table));
		storeTableConfigs();
	}

	MANGO_API void MangoDB::removeTable(std::string_view tableName)
	{
		std::vector<std::unique_ptr<Implementation::Table>> newTables;
		newTables.reserve(m_Tables.size() - 1);

		for (auto& table : m_Tables)
			if (table->getName() != tableName)
				newTables.push_back(std::move(table));
			else
				table->removeFiles();

		m_Tables = std::move(newTables);

		storeTableConfigs();
	}

	MANGO_API const_ptr<Implementation::Table> MangoDB::getTable(std::string_view tableName) const
	{
		for (const auto& tablePtr : m_Tables)
			if (tablePtr->getName() == tableName)
				return tablePtr.get();
		return nullptr;
	}

	MANGO_API ptr<Implementation::Table> MangoDB::getTable(std::string_view tableName)
	{
		for (const auto& tablePtr : m_Tables)
			if (tablePtr->getName() == tableName)
				return tablePtr.get();
		return nullptr;
	}

	MANGO_API const_ref<std::vector<std::unique_ptr<Implementation::Table>>> MangoDB::tables() const
	{
		return m_Tables;
	}

	MANGO_API void MangoDB::storeTableConfigs() const
	{
		std::fstream file(getConfigFilePath(), std::ios::out | std::ios::trunc | std::ios::binary);

		size_t tablesSize = m_Tables.size();
		serializePOD(file, &tablesSize);

		for (const auto& table : m_Tables)
		{
			size_t tableNameLength = table->getName().length() + 1;
			serializePOD(file, &tableNameLength);
			serializePOD(file, table->getName().data(), tableNameLength);

			table->serializeConfig();
		}

		file.close();
	}
#pragma endregion
	
#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API void MangoDB::loadTableConfigs()
	{
		std::fstream file(getConfigFilePath(), std::ios::in | std::ios::binary);

		size_t tablesSize = 0;
		deserializePOD(file, &tablesSize);
		m_Tables.reserve(tablesSize);

		for (size_t i = 0; i < tablesSize; ++i)
		{
			size_t tableNameLenth = 0;
			deserializePOD(file, &tableNameLenth);
			char buffer[MANGO_MAX_TABLE_NAME_LENGTH + 1]{};
			deserializePOD(file, buffer, tableNameLenth);

			std::vector<Implementation::Column> columns;
			auto& table = m_Tables.emplace_back(std::make_unique<Implementation::Table>(buffer, m_DirectoryPath, std::move(columns)));
			table->deserializeConfig();
		}

		file.close();
	}

	MANGO_PUBLIC_API const_ref<std::vector<Implementation::Row>> MangoDB::lastResult() const
	{
		return m_LastResult;
	}

	MANGO_PUBLIC_API const_ref<std::vector<std::string>> MangoDB::lastColumns() const
	{
		return m_LastColumns;
	}

	std::vector<Implementation::Row> MangoDB::extractLastResult()
	{
		std::vector<Implementation::Row> dummy(std::move(m_LastResult));
		std::vector<Implementation::Row>().swap(m_LastResult);
		return dummy;
	}

	MANGO_PUBLIC_API void MangoDB::disposeLastResult()
	{
		std::vector<Implementation::Row>().swap(m_LastResult);
	}

	void MangoDB::setWhereClause(Implementation::WhereClause whereClause)
	{
		m_WhereClause = whereClause;
	}

	void MangoDB::resetWhereClause()
	{
		m_WhereClause = &Implementation::RowFilters::allwaysTrue;
	}

	void MangoDB::setSetClause(Implementation::SetClause setClause)
	{
		m_SetClause = setClause;
	}

	void MangoDB::resetSetClause()
	{
		m_SetClause = &Implementation::RowFilters::doNothing;
	}

	MANGO_PUBLIC_API MangoDB::MangoDB(std::filesystem::path dataBaseDirectoryPath)
		: m_WhereClause(&Implementation::RowFilters::allwaysTrue), m_SetClause(&Implementation::RowFilters::doNothing)
	{
		m_DirectoryPath = std::move(dataBaseDirectoryPath);
		std::filesystem::create_directories(m_DirectoryPath);

		std::fstream file;
		file.open(getConfigFilePath(), std::ios::out | std::ios::app | std::ios::binary);
		file.close();
	}

	MANGO_PUBLIC_API MangoDB::~MangoDB()
	{
		storeTableConfigs();
	}

	namespace Implementation
	{
		MANGO_PUBLIC_API std::ostream& operator<<(std::ostream& out, const MangoDB& mango)
		{
			out << MANGO_STREAM_OPERATOR_HEADER << '\n';
			for (const auto& table : mango.tables())
				out << *table << MANGO_CLI_SEPARATOR_LINE << '\n';

			return out;
		}
	}
	
#pragma endregion
}
