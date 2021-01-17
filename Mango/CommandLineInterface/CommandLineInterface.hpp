#pragma once
#include "../Implementation/Queries/Queries.hpp"

namespace Mango
{
	class CommandLineInterface
	{
	private:
		static const std::array<std::unique_ptr<Implementation::Queries::IQuery>, 12> s_Queries;
		MangoDB m_DataBase;
		bool m_Running = true;

	private:
		void format(ref<std::string> sql) const;
		void help() const;

	public:
		void run();
		ref<MangoDB> dataBase();
		void execute(ref<std::string> sql, ref<MangoDB> dataBase);

		MANGO_PUBLIC_API CommandLineInterface(std::string_view dataBaseDirectoryPath);

		MANGO_PUBLIC_API CommandLineInterface() = default;
		MANGO_PUBLIC_API CommandLineInterface(const CommandLineInterface&) = delete;
		MANGO_PUBLIC_API CommandLineInterface(CommandLineInterface&&) noexcept = default;
		MANGO_PUBLIC_API CommandLineInterface& operator=(const CommandLineInterface&) = delete;
		MANGO_PUBLIC_API CommandLineInterface& operator=(CommandLineInterface&&) noexcept = default;
		MANGO_PUBLIC_API ~CommandLineInterface() = default;
	};
}