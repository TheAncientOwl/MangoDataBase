#include "standard_library.hpp"
#include "Timer.hpp"

namespace Mango::Implementation
{
	Timer::Seconds Timer::elapsedTime() const
	{	
		return std::chrono::duration_cast<Timer::Seconds>(std::chrono::high_resolution_clock::now() - m_Start);
	}

	Timer::Timer()
		: m_Start(std::chrono::high_resolution_clock::now())
	{
	}
}