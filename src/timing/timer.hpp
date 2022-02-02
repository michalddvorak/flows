#pragma once
#include <string>
#include <chrono>

std::string get_time_stamp();

struct timer
{
	using time_rep_t = std::chrono::time_point<std::chrono::steady_clock>::duration::rep;
	std::chrono::time_point<std::chrono::steady_clock> get();
	void start();
	void end();
	time_rep_t time();
	
	template<typename F, typename... Args>
	time_rep_t function_time(F func, Args&&... args)
	{
		start();
		func(std::forward<Args>(args)...);
		end();
		return time();
	}
 
 private:
	
	bool measuring = false;
	std::chrono::time_point<std::chrono::steady_clock> s, e;
};
