#include <stdexcept>
#include "timer.hpp"

std::chrono::time_point<std::chrono::steady_clock> timer::get()
{
	return std::chrono::steady_clock::now();
}

void timer::start()
{
	if(measuring)
		throw std::invalid_argument("Already measuring");
	s = get();
	measuring = true;
}

void timer::end()
{
	if(!measuring)
		throw std::invalid_argument("Not measuring");
	e = get();
	measuring = false;
}


std::chrono::time_point<std::chrono::steady_clock>::duration::rep timer::time()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();
}

std::string get_time_stamp()
{
	std::time_t t = std::time(nullptr);
	char buf[100];
	strftime(buf,sizeof buf, "%F_%T",std::localtime(&t));
	std::string result(buf);
	return result;
}
