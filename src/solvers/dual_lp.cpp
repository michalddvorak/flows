#include "dual_lp.hpp"
#include "gurobi_c++.h"

using flow_t = network::flow_t;
using vertex_t = network::vertex_t;

flow_t dual_lp::solve(const network& network)
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
	std::vector<GRBVar> y(n);
	GRBLinExpr obj(0);
	for(network::vertex_t u = 0; u < n; ++u)
		if(u != s && u != t)
			y[u] = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS);
	
	for(network::vertex_t u = 0; u < n; ++u)
	{
		for(auto&[v, c]: adj[u])
		{
			GRBVar var = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS);
			obj += var * c;
			if(u == s)
			{
				if(v == t)
					model.addConstr(var >= 1);
				else if(v != s)
					model.addConstr(y[v] + var >= 1);
			}
			else if(v == s)
			{
				if(u == t)
					model.addConstr(var >= -1);
				else if(u != s)
					model.addConstr(-y[u] + var >= -1);
			}
			else if(u == t)
			{
				if(v != t && v != s)
					model.addConstr(y[v] + var >= 0);
			}
			else if(v == t)
			{
				if(u != t && u != s)
					model.addConstr(-y[u] + var >= 0);
			}
			else
				model.addConstr(y[v] - y[u] + var >= 0);
		}
	}
	model.setObjective(obj, GRB_MINIMIZE);
	model.optimize();
	return obj.getValue();
}

std::string dual_lp::description()
{
	return "dual_lp";
}