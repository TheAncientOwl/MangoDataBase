#pragma once

namespace Utilities
{
	class ConsoleColorOutput
	{
	private:
		ConsoleColorOutput() {}
		static void setColor(int colorCode);

	public:
		static std::ostream& light_blue(std::ostream& out);
		static std::ostream& dark_blue(std::ostream& out);

		static std::ostream& light_aqua(std::ostream& out);
		static std::ostream& dark_aqua(std::ostream& out);

		static std::ostream& lime(std::ostream& out);
		static std::ostream& green(std::ostream& out);

		static std::ostream& light_red(std::ostream& out);
		static std::ostream& dark_red(std::ostream& out);

		static std::ostream& purple(std::ostream& out);
		static std::ostream& pink(std::ostream& out);

		static std::ostream& gold(std::ostream& out);
		static std::ostream& yellow(std::ostream& out);

		static std::ostream& light_gray(std::ostream& out);
		static std::ostream& dark_gray(std::ostream& out);
		static std::ostream& white(std::ostream& out);

		static std::ostream& reset(std::ostream& out);
	};
}

#define ccolor Utilities::ConsoleColorOutput