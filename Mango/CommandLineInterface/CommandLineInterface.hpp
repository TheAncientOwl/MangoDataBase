#pragma once
#include "../Implementation/Queries/Queries.hpp"

namespace Mango
{
	class CommandLineInterface
	{
	private:
		bool m_Select = false;
		bool m_Running = true;

	private:
		std::filesystem::path m_DataBaseDirectoryPath;
		static const std::array<std::unique_ptr<Implementation::Queries::AbstractQuery>, 11> s_Queries;
		void format(ref<std::string> sql) const;
		int digitsNo(int x) const;
		int digitsNo(float x) const;
		void displayResult(const_ref<std::vector<Implementation::Row>> rows, const_ref<std::vector<std::string>> columns) const;
		void help() const;

	public:
		void run();
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