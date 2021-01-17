#pragma once
#include "../Implementation/Queries/Queries.hpp"

namespace Mango
{
	class QueryExecutor
	{
	private:
		static const std::array<std::unique_ptr<Implementation::Queries::IQuery>, 10> s_Queries;
		void format(ref<std::string> sql) const;

	public:
		void execute(std::string&& sql, ref<MangoDB> dataBase);
	};
}