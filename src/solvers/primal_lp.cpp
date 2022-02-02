#include "primal_lp.hpp"
#include "gurobi_c++.h"

network::flow_t primal_lp::solve(const network& network)
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
	std::vector<GRBLinExpr> in(n, 0);
	std::vector<GRBLinExpr> out(n, 0);
	//std::vector<std::vector<GRBVar>> F(n);
	GRBLinExpr obj(0);
	for(network::vertex_t u = 0; u < n; ++u)
		for(auto&[v, c]: adj[u])
		{
			GRBVar var = model.addVar(0, c, 0, GRB_CONTINUOUS);
			//F[u].push_back(var);
			out[u] += var;
			in[v] += var;
			if(u == s)
				obj += var;
			if(v == s)
				obj -= var;
		}
	for(network::vertex_t u = 0; u < n; ++u)
		if(u != s && u != t)
			model.addConstr(in[u] == out[u]);
	model.setObjective(obj, GRB_MAXIMIZE);
	model.optimize();
	return obj.getValue();
}
std::string primal_lp::description()
{
	return "primal_lp";
}
