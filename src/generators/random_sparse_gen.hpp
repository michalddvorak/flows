#pragma once

#include "nwgenerator.hpp"

class random_sparse_gen : public nwgenerator
{
 public:
	random_sparse_gen(size_t n, size_t m, size_t cmin, size_t cmax);
	network gen(unsigned int seed) override;
 private:
	size_t n, m;
	size_t cmin, cmax;
};




