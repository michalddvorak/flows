#pragma once
#include "asolver.hpp"

class primal_lp : public asolver
{
 public:
	network::flow_t solve(const network &network) override;
	std::string description() override;
};




