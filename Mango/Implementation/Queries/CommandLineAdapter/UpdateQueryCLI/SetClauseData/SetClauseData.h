#pragma once
#include "../../../Queries.fwd.hpp"

#include "../../../../Table/Table.hpp"
#include "../../../../Row/Row.hpp"

namespace Mango::Implementation::Queries::CommandLineAdapter
{
	void setClauseCLI(ref<Row> row);
	class SetClauseData
	{
	private:
		std::vector<std::pair<std::string, std::string>> m_Data;

		std::vector<int> m_SetIndexes;
		Row m_Row;

		void clear();

		void parseFrom(std::string_view clause);

		void validate(const_ptr<Table> table) const;

		void extractRow(ptr<Table> table);

	public:
		static ref<SetClauseData> Instance();

	private:
		SetClauseData() = default;

	public:
		SetClauseData(const SetClauseData&) = delete;
		SetClauseData(SetClauseData&&) noexcept = delete;
		SetClauseData& operator=(const SetClauseData&) = delete;
		SetClauseData& operator=(SetClauseData&&) noexcept = delete;
		~SetClauseData() = default;

	private:
		friend class UpdateQueryCLI;
		friend void setClauseCLI(ref<Row> row);
	};
}