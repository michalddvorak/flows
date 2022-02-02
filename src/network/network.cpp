#include "network.hpp"

network::network(size_t _n, network::vertex_t _s, network::vertex_t _t) : adj(_n), n(_n), s(_s), t(_t) { }


void network::add_edge(vertex_t u, vertex_t v, flow_t c)
{
	adj[u].emplace_back(v,c);
}

network::edge::edge(vertex_t _v, flow_t _c) : v(_v), c(_c) { }