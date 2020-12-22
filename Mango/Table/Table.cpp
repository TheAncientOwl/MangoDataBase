#include "standard_library.hpp"
#include "Table.hpp"

namespace Mango
{
	Table::Table(std::string name, const_ref<std::filesystem::path> dataBaseDirectoryPath, std::vector<Column>&& columns)
	{
		m_Name = std::move(name);
		m_DirectoryPath = dataBaseDirectoryPath / m_Name;

		m_Columns = std::move(columns);
		int index = 0;
		for (const auto& column : m_Columns)
			m_ColumnIndexes.emplace(column.name(), index), index++;
	}

	inline std::filesystem::path Table::getConfigFilePath() const
	{
		return m_DirectoryPath / (m_Name + MANGO_CONFIG_EXTENSION);
	}

	inline std::filesystem::path Table::getDataFilePath() const
	{
		return m_DirectoryPath / (m_Name + MANGO_DATA_EXTENSION);
	}

	void PRIVATE_API Table::createFiles()
	{
		std::filesystem::create_directories(m_DirectoryPath);

		std::fstream file;

		file.open(getConfigFilePath(), std::ios::out | std::ios::app);
		file.close();

		file.open(getDataFilePath(), std::ios::out | std::ios::app);
		file.close();
	}

	void PRIVATE_API Table::removeFiles()
	{
		std::filesystem::remove_all(m_DirectoryPath);
	}

	void PRIVATE_API Table::clearDataFile()
	{
		std::fstream file;
		file.open(getDataFilePath(), std::ios::out | std::ios::trunc);
		file.close();
	}

	void PRIVATE_API Table::serializeConfig()
	{
		std::fstream file(getConfigFilePath(), std::ios::out | std::ios::trunc | std::ios::binary);

		size_t columnsSize = m_Columns.size();
		serializePOD(file, &columnsSize);

		for (const auto& column : m_Columns)
			column.serialize(file);

		file.close();
	}

	void PRIVATE_API Table::deserializeConfig()
	{
		std::fstream file(getConfigFilePath(), std::ios::in | std::ios::binary);

		size_t columnsSize = 0;
		deserializePOD(file, &columnsSize);
		m_Columns.reserve(columnsSize);

		for (int index = 0; index < columnsSize; ++index)
		{
			auto& column = m_Columns.emplace_back();
			column.deserialize(file);

			m_ColumnIndexes.emplace(column.name(), index);
		}

		file.close();
	}

	void PRIVATE_API Table::insertRow(const_ref<Row> row)
	{
		std::fstream file(getDataFilePath(), std::ios::out | std::ios::app | std::ios::binary);

		serializePOD(file, row.data(), row.size());

		file.close();
	}

	std::string_view Table::name() const
	{
		return m_Name;
	}

	const_ref<std::vector<Column>> Table::columns() const
	{
		return m_Columns;
	}

	const_ptr<Column> Table::getColumn(std::string_view columnName) const
	{
		for (const auto& column : m_Columns)
			if (column.name() == columnName)
				return &column;
		return nullptr;
	}

	ptr<Column> Table::getColumn(std::string_view columnName)
	{
		for (auto& column : m_Columns)
			if (column.name() == columnName)
				return &column;
		return nullptr;
	}

	const_ref<Column> Table::getColumn(int index) const
	{
		assert(index >= 0 && index < m_Columns.size());
		return m_Columns[index];
	}

	ref<Column> Table::getColumn(int index)
	{
		assert(index >= 0 && index < m_Columns.size());
		return m_Columns[index];
	}

	size_t Table::getColumnIndex(std::string_view columnName) const
	{
		return m_ColumnIndexes.at(columnName);
	}

	std::shared_ptr<RowConfiguration> PRIVATE_API Table::makeSharedRowConfiguration() const
	{
		auto rowConfig = std::make_shared<RowConfiguration>();
		for (const auto& column : m_Columns)
			rowConfig->pushBack(column.size(), column.dataType());
		return rowConfig;
	}

	RowConfiguration PRIVATE_API Table::makeRowConfiguration() const
	{
		RowConfiguration rowConfig;
		for (const auto& column : m_Columns)
			rowConfig.pushBack(column.size(), column.dataType());
		return rowConfig;
	}

	TableIterator PRIVATE_API Table::makeIterator()
	{
		return TableIterator(getDataFilePath(), makeSharedRowConfiguration());
	}

	std::ostream& operator<<(std::ostream& out, const Table& table)
	{
		out << ccolor::dark_gray << "[";
		out << ccolor::dark_red << std::string(table.name());
		out << ccolor::dark_gray << "]\n";

		for (const auto& column : table.columns())
			out << column << '\n';

		out << ccolor::green << "=======================================";

		return out;
	}

}