#pragma once
#include "../../../../MangoAPI.fwd.hpp"

namespace Mango::Implementation::Queries
{
	class ScriptExecutor
	{
	private:
		std::string m_FileName;

	public:
		ScriptExecutor(std::string fileName);
		ScriptExecutor(const ScriptExecutor&) = default;
		ScriptExecutor(ScriptExecutor&&) noexcept = default;
		ScriptExecutor& operator=(const ScriptExecutor&) = default;
		ScriptExecutor& operator=(ScriptExecutor&&) noexcept = default;
		~ScriptExecutor() = default;

	public:
		void execute(ref<MangoDB> dataBase, ref<CommandLineInterface> cli) const;
	};
}