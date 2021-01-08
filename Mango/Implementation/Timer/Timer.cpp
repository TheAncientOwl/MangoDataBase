#include "standard_library.hpp"
#include "Timer.hpp"

namespace Mango::Implementation
{
	std::pair<Timer::Seconds, Timer::Milliseconds> Timer::elapsedTime() const
	{	
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds duration = end - m_Start;
		return std::make_pair
		(
			std::chrono::duration_cast<Timer::Seconds>(duration),
			std::chrono::duration_cast<Timer::Milliseconds>(duration)
		);
	}

	Timer::Timer()
		: m_Start(std::chrono::high_resolution_clock::now())
	{
	}
}