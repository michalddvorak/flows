#pragma once
#include "asolver.hpp"

class exp_lp : public asolver
{
 public:
	network::flow_t solve(const network &network) override;
	std::string description() override;
	size_t get_lp_runs();
 private:
	size_t lp_runs = 0;
};
