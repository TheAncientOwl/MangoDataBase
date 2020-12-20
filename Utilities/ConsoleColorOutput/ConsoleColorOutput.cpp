#include "standard_library.hpp"
#include "ConsoleColorOutput.hpp"

#include <Windows.h>

namespace Utilities
{
	void ConsoleColorOutput::setColor(int colorCode)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, colorCode);
	}

	std::ostream& ConsoleColorOutput::dark_blue(std::ostream& out)
	{
		ConsoleColorOutput::setColor(1);
		return out;
	}

	std::ostream& ConsoleColorOutput::green(std::ostream& out)
	{
		ConsoleColorOutput::setColor(2);
		return out;
	}

	std::ostream& ConsoleColorOutput::dark_aqua(std::ostream& out)
	{
		ConsoleColorOutput::setColor(3);
		return out;
	}

	std::ostream& ConsoleColorOutput::dark_red(std::ostream& out)
	{
		ConsoleColorOutput::setColor(4);
		return out;
	}

	std::ostream& ConsoleColorOutput::purple(std::ostream& out)
	{
		ConsoleColorOutput::setColor(5);
		return out;
	}

	std::ostream& ConsoleColorOutput::gold(std::ostream& out)
	{
		ConsoleColorOutput::setColor(6);
		return out;
	}

	std::ostream& ConsoleColorOutput::light_gray(std::ostream& out)
	{
		ConsoleColorOutput::setColor(7);
		return out;
	}

	std::ostream& ConsoleColorOutput::dark_gray(std::ostream& out)
	{
		ConsoleColorOutput::setColor(8);
		return out;
	}

	std::ostream& ConsoleColorOutput::light_blue(std::ostream& out)
	{
		ConsoleColorOutput::setColor(9);
		return out;
	}

	std::ostream& ConsoleColorOutput::lime(std::ostream& out)
	{
		ConsoleColorOutput::setColor(10);
		return out;
	}

	std::ostream& ConsoleColorOutput::light_aqua(std::ostream& out)
	{
		ConsoleColorOutput::setColor(11);
		return out;
	}

	std::ostream& ConsoleColorOutput::light_red(std::ostream& out)
	{
		ConsoleColorOutput::setColor(12);
		return out;
	}

	std::ostream& ConsoleColorOutput::pink(std::ostream& out)
	{
		ConsoleColorOutput::setColor(13);
		return out;
	}

	std::ostream& ConsoleColorOutput::yellow(std::ostream& out)
	{
		ConsoleColorOutput::setColor(14);
		return out;
	}

	std::ostream& ConsoleColorOutput::white(std::ostream& out)
	{
		ConsoleColorOutput::setColor(15);
		return out;
	}

	std::ostream& ConsoleColorOutput::reset(std::ostream& out)
	{
		out << ccolor::light_gray;
		return out;
	}
}