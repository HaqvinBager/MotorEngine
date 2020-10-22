#pragma once
#include <chrono>
#include <iostream>
#include <functional>

template<auto Callback, class SourceType, typename...Params>
static void LogTime(SourceType* instancePointer, Params&&...p) 
{
	auto start = std::chrono::high_resolution_clock::now();

	std::invoke(Callback, instancePointer, std::forward<Params>(p)...);

	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Function took: " << elapsed.count() << " s to complete." << std::endl;
}