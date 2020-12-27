#pragma once
#include "../Implementation/Queries/Queries.hpp"

namespace Mango
{
	class QueryExecutor
	{
	private:
		static const std::array<std::unique_ptr<Implementation::Queries::AbstractQuery>, 8> s_Queries;

	public:
		void execute(std::string&& sql, ref<MangoDB> dataBase);
	};
}