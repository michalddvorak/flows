#pragma once

#include <random>
#include "../network/network.hpp"

class nwgenerator
{
 public:
	virtual ~nwgenerator() = default;
	virtual network gen(unsigned int) = 0;
 protected:
	std::mt19937 rng;
};