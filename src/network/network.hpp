#pragma once

#include <limits>
#include <vector>

struct network
{
	using vertex_t = int;
	using flow_t = long long int;
	
	network(size_t _n, vertex_t _s, vertex_t _t);
	void add_edge(vertex_t u, vertex_t v, flow_t c);
	struct edge
	{
		edge(vertex_t v, flow_t c);
		vertex_t v;
		flow_t c;
	};
	std::vector<std::vector<edge>> adj;
	const size_t n;
	const vertex_t s;
	const vertex_t t;
	const flow_t INF = std::numeric_limits<flow_t>::max();
};




