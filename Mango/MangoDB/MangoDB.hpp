#pragma once
#include "../Table/Table.hpp"

#define MANGO_MAX_TABLE_NAME_LENGTH 50

namespace Mango
{
	class MangoDB
	{
	private: /// Fields
		std::filesystem::path m_DirectoryPath;
		std::vector<std::unique_ptr<Table>> m_Tables;

	private: /// Private methods
		inline std::filesystem::path getConfigFilePath() const;

	private: /// API
		/// <summary>
		/// Add table to internal data structure.
		/// Create its files.
		/// Store new configuration.
		/// </summary>
		void PRIVATE_API addTable(std::unique_ptr<Table> table);

		/// <summary>
		/// Remove table from internal data structure.
		/// Remove its files.
		/// Store new configuration.
		/// </summary>
		void PRIVATE_API removeTable(std::string_view tableName);

	public: /// Interface
		void storeTables() const;
		void loadTables();

		const_ref<std::vector<std::unique_ptr<Table>>> tables() const;

	public: /// Constructors
		MangoDB(std::filesystem::path dataBaseDirectoryPath);

		MangoDB(const MangoDB&) = delete;
		MangoDB(MangoDB&&) noexcept = default;
		MangoDB& operator=(const MangoDB&) = delete;
		MangoDB& operator=(MangoDB&&) noexcept = default;
		~MangoDB() = default;
	};
}