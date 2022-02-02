#pragma once

#include <string>
#include "../network/network.hpp"

class asolver
{
 public:
	virtual network::flow_t solve(const network& network) = 0;
	virtual std::string description() = 0;
};