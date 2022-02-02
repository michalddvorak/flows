#include <map>
#include "random_sparse_gen.hpp"

using vertex_t = network::vertex_t;
using flow_t = network::flow_t;

random_sparse_gen::random_sparse_gen(size_t _n, size_t _m, size_t _cmin, size_t _cmax) : n(_n), m(_m), cmin(_cmin), cmax(_cmax) { }

network random_sparse_gen::gen(unsigned int seed)
{
	rng.seed(seed);
	if(m > n * n)
		throw std::invalid_argument("too many edges");
	network nw(n, 0, n - 1);
	std::uniform_int_distribution<network::flow_t> cap(cmin, cmax);
	std::uniform_int_distribution<network::vertex_t> vertices(0, n - 1);
	std::map<std::pair<vertex_t, vertex_t>, network::flow_t> edges;
	while(edges.size() != m)
	{
		edges[{vertices(rng), vertices(rng)}] = cap(rng);
	}
	for(auto&[e, c]: edges)
	{
		auto&[u, v] = e;
		nw.add_edge(u, v, c);
	}
	return nw;
}
