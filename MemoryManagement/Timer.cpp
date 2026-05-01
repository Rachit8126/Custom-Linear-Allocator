#include <chrono>
#include <iostream>

#include "Timer.h"

Timer::Timer()
{
	m_StartTimepont = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	auto endTimePoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepont).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

	auto duration = end - start;
	double ms = duration * 0.001;

	std::cout << "Time taken: " << ms << " ms" << std::endl;
}