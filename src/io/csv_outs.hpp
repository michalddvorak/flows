#pragma once
#include <type_traits>
#include <ostream>

template<size_t Ncols>
class csv_outs
{
 public:
	csv_outs(std::ostream& _os) : os(_os) { }
	
	template<typename ...Ts>
	csv_outs(std::ostream& _os, const Ts& ... columns) : csv_outs(_os)
	{
		write_line(columns...);
	}
	template<typename T, typename ...Ts>
	std::enable_if_t<sizeof...(Ts) + 1 == Ncols, std::ostream&> write_line(const T& first, const Ts& ... cols)
	{
		os << first;
		return ((os << ',' << cols), ..., (os << std::endl));
	}
 
 private:
	std::ostream& os;
};

template<typename...Ts>
csv_outs(std::ostream&, const Ts& ...) -> csv_outs<sizeof...(Ts)>;