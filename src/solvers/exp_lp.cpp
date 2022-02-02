#include <queue>
#include "gurobi_c++.h"
#include "exp_lp.hpp"

using flow_t = network::flow_t;
using vertex_t = network::vertex_t;
size_t exp_lp::get_lp_runs()
{
	return lp_runs;
}

network::flow_t exp_lp::solve(const network& network)
{
	GRBEnv env(true);
	env.set(GRB_IntParam_LogToConsole, 0);
	env.set(GRB_StringParam_LogFile, "logs/gurobi-log.txt");
	env.start();
	GRBModel model(env);
	const auto& n = network.n;
	const auto& adj = network.adj;
	const auto& s = network.s;
	const auto& t = network.t;
	std::vector<std::vector<GRBVar>> vars(n, std::vector<GRBVar>(n));
	GRBLinExpr obj(0);
	for(network::vertex_t u = 0; u < n; ++u)
		for(auto&[v, c]: adj[u])
		{
			GRBVar var = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS);
			vars[u][v] = var;
			obj += var * c;
		}
	model.setObjective(obj, GRB_MINIMIZE);
	
	
	std::vector<bool> seen(n);
	std::vector<vertex_t> prev(n);
	std::vector<double> dist(n);
	auto dijkstra = [&]() {
		struct vd_pair
		{
			vertex_t v;
			double d;
			bool operator >(const vd_pair& other) const
			{
				return d > other.d;
			}
		};
		const double INF = n * n * n * 42.0f; //surely longer than longest path in the graph
		std::priority_queue<vd_pair, std::vector<vd_pair>, std::greater<vd_pair>> q;
		seen.assign(n, false);
		for(auto& d: dist)
			d = INF;
		dist[s] = 0;
		q.push({s, 0});
		while(!q.empty())
		{
			auto[u, d] = q.top();
			q.pop();
			if(seen[u])
				continue;
			seen[u] = true;
			for(auto&[v, c]: adj[u])
			{
				auto dd = d + vars[u][v].get(GRB_DoubleAttr_X);
				if(dd < dist[v])
				{
					dist[v] = dd;
					q.push({v, dd});
					prev[v] = u;
				}
			}
		}
	};
	
	lp_runs = 0;
	while(true)
	{
		lp_runs++;
		model.optimize();
		dijkstra();
		if(dist[t]+1e-10 < 1)
		{
			GRBLinExpr constr(0);
			vertex_t v = t;
			while(v != s)
			{
				constr += vars[prev[v]][v];
				v = prev[v];
			}
			model.addConstr(constr >= 1);
		}
		else
			return obj.getValue();
	}
}


std::string exp_lp::description()
{
	return "exp_lp";
}
