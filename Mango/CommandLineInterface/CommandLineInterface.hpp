#pragma once
#include "../MangoAPI.hpp"

namespace Mango
{
	using namespace Queries;
	using namespace Exceptions;

	class CommandLineInterface
	{
	private:
		static const std::array<std::unique_ptr<AbstractQuery>, 5> s_Queries;

	public:
		void run();
	};
}