#include "standard_library.hpp"
#include "Table.hpp"

#include "../BinaryUtils/BinaryUtils.hpp"
using namespace Mango::Implementation::BinaryUtils;

namespace Mango::Implementation
{
#pragma region MANGO_PRIVATE_API
	MANGO_PRIVATE_API std::filesystem::path Table::getConfigFilePath() const
	{
		return m_DirectoryPath / (m_Name + MANGO_CONFIG_EXTENSION);
	}

	MANGO_PRIVATE_API std::filesystem::path Table::getDataFilePath() const
	{
		return m_DirectoryPath / (m_Name + MANGO_DATA_EXTENSION);
	}

	MANGO_PRIVATE_API const_ref<std::filesystem::path> Table::getDirectoryPath() const
	{
		return m_DirectoryPath;
	}

	MANGO_PRIVATE_API void Table::createFiles()
	{
		std::filesystem::create_directories(m_DirectoryPath);

		std::fstream file;

		file.open(getConfigFilePath(), std::ios::out | std::ios::app);
		file.close();

		file.open(getDataFilePath(), std::ios::out | std::ios::app);
		file.close();
	}

	MANGO_PRIVATE_API void Table::removeFiles()
	{
		std::filesystem::remove_all(m_DirectoryPath);
	}

	MANGO_PRIVATE_API void Table::clearDataFile()
	{
		std::fstream file;
		file.open(getDataFilePath(), std::ios::out | std::ios::trunc);
		file.close();
	}

	MANGO_PRIVATE_API void Table::serializeConfig()
	{
		std::fstream file(getConfigFilePath(), std::ios::out | std::ios::trunc | std::ios::binary);

		size_t columnsSize = m_Columns.size();
		serializePOD(file, &columnsSize);

		for (const auto& column : m_Columns)
			column.serialize(file);

		file.close();
	}

	MANGO_PRIVATE_API void Table::deserializeConfig()
	{
		std::fstream file(getConfigFilePath(), std::ios::in | std::ios::binary);

		size_t columnsSize = 0;
		deserializePOD(file, &columnsSize);
		m_Columns.reserve(columnsSize);

		for (int index = 0; index < columnsSize; ++index)
		{
			auto& column = m_Columns.emplace_back();
			column.deserialize(file);
		}

		file.close();
	}

	MANGO_PRIVATE_API void Table::insertRow(const_ref<Row> row)
	{
		std::fstream file(getDataFilePath(), std::ios::out | std::ios::app | std::ios::binary);

		serializePOD(file, row.data(), row.size());

		file.close();
	}

	MANGO_PRIVATE_API const_ref<std::vector<Column>> Table::columns() const
	{
		return m_Columns;
	}

	MANGO_PRIVATE_API std::string_view Table::getName() const
	{
		return m_Name;
	}

	MANGO_PRIVATE_API const_ptr<Column> Table::getColumn(std::string_view columnName) const
	{
		size_t index = getColumnIndex(columnName);
		if (index == -1)
			return nullptr;
		else
			return &m_Columns[index];
	}

	MANGO_PRIVATE_API ptr<Column> Table::getColumn(std::string_view columnName)
	{
		size_t index = getColumnIndex(columnName);
		if (index == -1)
			return nullptr;
		else
			return &m_Columns[index];
	}

	MANGO_PRIVATE_API const_ref<Column> Table::getColumn(int index) const
	{
		assert(index >= 0 && index < m_Columns.size());
		return m_Columns[index];
	}

	MANGO_PRIVATE_API ref<Column> Table::getColumn(int index)
	{
		assert(index >= 0 && index < m_Columns.size());
		return m_Columns[index];
	}

	MANGO_PRIVATE_API size_t Table::getColumnIndex(std::string_view columnName) const
	{
		for (size_t index = 0, size = m_Columns.size(); index < size; ++index)
			if (m_Columns[index].name() == columnName)
				return index;
		return -1;
	}

	MANGO_PRIVATE_API std::shared_ptr<RowConfiguration> Table::makeSharedRowConfiguration() const
	{
		auto rowConfig = std::make_shared<RowConfiguration>();
		for (const auto& column : m_Columns)
			rowConfig->pushBack(column.size(), column.dataType());
		return rowConfig;
	}

	MANGO_PRIVATE_API RowConfiguration Table::makeRowConfiguration() const
	{
		RowConfiguration rowConfig;
		for (const auto& column : m_Columns)
			rowConfig.pushBack(column.size(), column.dataType());
		return rowConfig;
	}

	MANGO_PRIVATE_API TableIterator Table::makeIterator()
	{
		return TableIterator(getDataFilePath(), makeSharedRowConfiguration());
	}

	MANGO_PRIVATE_API ConstTableIterator Table::makeConstIterator() const
	{
		return ConstTableIterator(getDataFilePath(), makeSharedRowConfiguration());
	}
#pragma endregion

#pragma region MANGO_PUBLIC_API
	MANGO_PUBLIC_API Table::Table(std::string name, const_ref<std::filesystem::path> dataBaseDirectoryPath, std::vector<Column>&& columns)
	{
		m_Name = std::move(name);
		m_DirectoryPath = dataBaseDirectoryPath / m_Name;

		m_Columns = std::move(columns);
	}

	MANGO_PUBLIC_API std::ostream& operator<<(std::ostream& out, const Table& table)
	{
		out << ccolor::dark_gray << "[";
		out << ccolor::dark_red << std::string(table.getName());
		out << ccolor::dark_gray << "]\n";

		for (const auto& column : table.columns())
			out << column << '\n';

		out << ccolor::green << "=======================================";

		return out;
	}
#pragma endregion

}