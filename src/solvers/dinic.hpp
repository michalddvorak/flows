#pragma once
#include "asolver.hpp"

class dinic : public asolver
{
 public:
	network::flow_t solve(const network &network) override;
	std::string description() override;
};




