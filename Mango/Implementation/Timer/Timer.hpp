#pragma once
#include <chrono>

namespace Mango::Implementation
{
	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
		
	public:
		using Seconds = std::chrono::duration<long double>;
		using Milliseconds = std::chrono::duration<long double, std::milli>;
		std::pair<Seconds, Milliseconds> elapsedTime() const;

	public:
		Timer();
		Timer(const Timer&) = default;
		Timer(Timer&&) noexcept = default;
		Timer& operator=(const Timer&) = default;
		Timer& operator=(Timer&&) noexcept = default;
		~Timer() = default;
	};
}