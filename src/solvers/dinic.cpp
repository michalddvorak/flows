#include <queue>
#include "dinic.hpp"

using flow_t = network::flow_t;
using vertex_t = network::vertex_t;

struct flow_edge
{
	flow_edge(vertex_t _v, flow_t _c) : v(_v), c(_c) { }
	vertex_t v;
	flow_t c, f = 0;
};

struct dinic_solver
{
	dinic_solver(size_t n, vertex_t _s, vertex_t _t) : adj(n), level(n), starts(n), s(_s), t(_t) { }
	void add_edge(vertex_t u, vertex_t v, flow_t c)
	{
		adj[u].push_back(edges.size());
		edges.emplace_back(v, c);
		adj[v].push_back(edges.size());
		edges.emplace_back(u, 0);
	}
	bool bfs()
	{
		for(auto& l: level)
			l = -1;
		level[s] = 0;
		q.push(s);
		while(!q.empty())
		{
			vertex_t u = q.front();
			q.pop();
			for(size_t edge_id: adj[u])
			{
				auto&[v, c, f] = edges[edge_id];
				if(level[v] == -1 && f < c)
				{
					level[v] = level[u] + 1;
					q.push(v);
				}
			}
		}
		return level[t] >= 0;
	}
	flow_t dfs(vertex_t u, flow_t flow)
	{
		if(u == t)
			return flow;
		if(level[u] >= level[t] || flow == 0)
			return 0;
		for(; starts[u] != adj[u].end(); ++starts[u])
		{
			size_t edge_id = *starts[u];
			auto&[v, c, f] = edges[edge_id];
			if(level[v] == level[u] + 1 && f < c)
			{
				flow_t forward_flow = dfs(v, std::min(flow, c - f));
				if(forward_flow > 0)
				{
					edges[edge_id].f += forward_flow;
					edges[edge_id ^ 1].f -= forward_flow;
					return forward_flow;
				}
			}
		}
		return 0;
	}
	
	flow_t solve_flows()
	{
		flow_t f = 0;
		while(bfs())
		{
			size_t i = 0;
			for(auto& adj_list: adj)
				starts[i++] = adj_list.begin();
			while(flow_t flow = dfs(s, std::numeric_limits<flow_t>::max()))
				f += flow;
		}
		return f;
	}
	std::vector<flow_edge> edges;
	std::vector<std::vector<size_t>> adj;
	std::vector<int> level;
	std::queue<vertex_t> q;
	std::vector<std::vector<size_t>::iterator> starts;
	vertex_t s, t;
};

flow_t dinic::solve(const network& network)
{
	const auto& n = network.n;
	const auto& adj = network.adj;
	const auto& s = network.s;
	const auto& t = network.t;
	dinic_solver solver(n, s, t);
	
	for(network::vertex_t u = 0; u < n; ++u)
		for(auto&[v, c]: adj[u])
			solver.add_edge(u, v, c);
	
	return solver.solve_flows();
}
std::string dinic::description()
{
	return "dinic";
}
