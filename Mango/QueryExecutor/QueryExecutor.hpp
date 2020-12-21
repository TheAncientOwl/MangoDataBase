#pragma once
#include "../Queries/Queries.hpp"

namespace Mango
{
	using namespace Queries;
	class QueryExecutor
	{
	private:
		static const std::array<std::unique_ptr<Queries::AbstractQuery>, 1> s_Queries;

	public:
		void execute(std::string&& sql, ref<MangoDB> dataBase);
	};
}