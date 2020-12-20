#pragma once
namespace Mango
{
	class MangoDB;
}

#include "../Column/Column.hpp"

#define MANGO_CONFIG_EXTENSION ".mangocfg"
#define MANGO_DATA_EXTENSION   ".mangodb"

namespace Mango
{
	class Table
	{
	private: /// Friends
		friend class MangoDB;

	private: /// Fields
		std::string m_Name;
		std::filesystem::path m_DirectoryPath;
		
		std::vector<Column> m_Columns;
		std::map<std::string_view, size_t> m_ColumnIndexes;

	private: /// Private methods
		inline std::filesystem::path getConfigFilePath() const;
		inline std::filesystem::path getDataFilePath() const;

	private: /// API
		void PRIVATE_API createFiles();
		void PRIVATE_API removeFiles();
		void PRIVATE_API clearDataFile();
		void PRIVATE_API serializeConfig();
		void PRIVATE_API deserializeConfig();

	public: /// Interface
		std::string_view name() const;
		const_ref<std::vector<Column>> columns() const;

	public: /// Constructors
		Table(std::string name, const_ref<std::filesystem::path> dataBaseDirectoryPath, std::vector<Column>&& columns);

		Table() = default;
		Table(const Table&) = delete;
		Table(Table&&) noexcept = default;
		Table& operator=(const Table&) = delete;
		Table& operator=(Table&&) noexcept = default;
		~Table() = default;
	};
}