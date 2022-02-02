#include <random>
#include <algorithm>
#include "random_dense_gen.hpp"


using vertex_t = network::vertex_t;
using flow_t = network::flow_t;

random_dense_gen::random_dense_gen(size_t _n, size_t _m, size_t _cmin, size_t _cmax) : n(_n), m(_m), cmin(_cmin), cmax(_cmax) { }

network random_dense_gen::gen(unsigned int seed)
{
	rng.seed(seed);
	if(m > n * n)
		throw std::invalid_argument("too many edges");
	network nw(n, 0, n - 1);
	std::uniform_int_distribution<network::flow_t> cap(cmin, cmax);
	std::vector<std::tuple<vertex_t, vertex_t, network::flow_t>> edges;
	for(size_t i = 0; i < n; ++i)
		for(size_t j = 0; j < n; ++j)
			edges.emplace_back(i, j, cap(rng));
	
	std::shuffle(edges.begin(), edges.end(), rng);
	for(size_t i = 0; i < m; ++i)
	{
		const auto&[u, v, c] = edges[i];
		nw.add_edge(u, v, c);
	}
	return nw;
}
