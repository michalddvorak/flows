#include <iostream>
#include <fstream>
#include <cassert>
#include "network/network.hpp"
#include "solvers/primal_lp.hpp"
#include "solvers/dinic.hpp"
#include "solvers/dual_lp.hpp"
#include "solvers/exp_lp.hpp"
#include "timing/timer.hpp"
#include "io/csv_outs.hpp"
#include "generators/random_dense_gen.hpp"
#include "generators/random_sparse_gen.hpp"

void basic_test(asolver& s)
{
	{
		network n(7, 3, 4);
		n.add_edge(3, 2, 1);
		n.add_edge(3, 1, 1);
		n.add_edge(1, 6, 1);
		n.add_edge(6, 2, 1);
		n.add_edge(6, 4, 1);
		n.add_edge(5, 1, 1);
		n.add_edge(5, 4, 1);
		n.add_edge(2, 5, 1);
		assert(s.solve(n) == 2);
	}
	{
		network n(8, 0, 7);
		n.add_edge(0, 1, 1);
		n.add_edge(1, 2, 1);
		n.add_edge(2, 3, 1);
		n.add_edge(3, 7, 1);
		n.add_edge(0, 4, 1);
		n.add_edge(4, 5, 1);
		n.add_edge(4, 3, 1);
		n.add_edge(5, 6, 1);
		n.add_edge(6, 7, 1);
		assert(s.solve(n) == 2);
	}
	{
		network n(6, 0, 5);
		n.add_edge(0, 1, 10);
		n.add_edge(0, 2, 10);
		n.add_edge(1, 2, 2);
		n.add_edge(2, 4, 9);
		n.add_edge(1, 4, 8);
		n.add_edge(1, 3, 4);
		n.add_edge(4, 3, 6);
		n.add_edge(3, 5, 10);
		n.add_edge(4, 5, 10);
		assert(s.solve(n) == 19);
	}
}

void dense_test(int n, asolver& s, network::flow_t cmin, network::flow_t cmax)
{
	std::ofstream ofs("out/dense" + std::to_string(n) + "-" + s.description() + "-" + std::to_string(cmin) + "-" + std::to_string(cmax) + "#" + get_time_stamp() + ".csv");
	csv_outs csv(ofs, "n", "m", s.description());
	timer t;
	for(int m = n; m <= n * n; m += 100)
	{
		std::cout << "m = " << m << std::endl;
		random_dense_gen g(n, m, cmin, cmax);
		network nw = g.gen(42);
		t.start();
		s.solve(nw);
		t.end();
		auto time = t.time();
		csv.write_line(n, m, time);
		if(time >= 5'000'000'000)
			break;
	}
}

void test_sparse(int c, asolver& s, network::flow_t cmin, network::flow_t cmax)
{
	std::ofstream ofs("out/sparse" + std::to_string(c) + "-" + s.description() + "-" + std::to_string(cmin) + "-" + std::to_string(cmax) + "#" + get_time_stamp() + ".csv");
	csv_outs csv(ofs, "n", "m", "lp_runs", s.description());
	timer t;
	for(int n = 50; n <= 3000; n += 5)
	{
		std::cout << "n = " << n << std::endl;
		int m = c * n;
		random_sparse_gen g(n, m, cmin, cmax);
		network nw = g.gen(42);
		t.start();
		s.solve(nw);
		t.end();
		auto time = t.time();
		csv.write_line(n, m, dynamic_cast<exp_lp&>(s).get_lp_runs(), time);
		if(time >= 5'000'000'000)
			break;
	}
}


void test_algos()
{
	dinic s1;
	primal_lp s2;
	dual_lp s3;
	exp_lp s4;
	basic_test(s1);
	basic_test(s2);
	basic_test(s3);
	basic_test(s4);
	random_dense_gen g(50, 1500, 50000, 100000);
	for(int i = 0; i < 100; ++i)
	{
		std::cout << i << std::endl;
		network n = g.gen(i);
		
		auto dinicf = s1.solve(n);
		auto plp = s2.solve(n);
		auto dlp = s3.solve(n);
		auto explp = s4.solve(n);
		std::cout << s4.get_lp_runs() << std::endl;
		
		assert(dinicf == plp);
		assert(plp == dlp);
		assert(dlp == explp);
	}
}


int main()
{
	//test_algos();
	
	dinic s1;
	primal_lp s2;
	dual_lp s3;
	//exp_lp s4;
	
	
	/*for(int c = 1; c <= 15; c++)
	{
		test_sparse(c, s4, 1, 100);
		test_sparse(c, s4, 1, 1);
		test_sparse(c, s1, 1, 1);
		test_sparse(c, s2, 1, 1);
		test_sparse(c, s3, 1, 1);
	}*/

	/*for(int n = 100; n <= 1000; n += 100)
	{
		dense_test(n, s1, 50000, 100000);
		dense_test(n, s1, 1, 100);
		dense_test(n, s1, 1, 1);
		dense_test(n, s2, 50000, 100000);
		dense_test(n, s2, 1, 100);
		dense_test(n, s2, 1, 1);
		dense_test(n, s3, 50000, 100000);
		dense_test(n, s3, 1, 100);
		dense_test(n, s3, 1, 1);
	}*/
	for(int n = 500; n <= 1000; n += 100)
	{
		if(n >= 700)
		{
		dense_test(n,s1,50000,100000);
		dense_test(n,s1,1,100);
		dense_test(n,s1,1,1);
		}
		dense_test(n,s2,50000,100000);
		dense_test(n,s2,1,100);
		dense_test(n,s2,1,1);
		dense_test(n,s3,50000,100000);
		dense_test(n,s3,1,100);
		dense_test(n,s3,1,1);
	
	}	
	
	return 0;
}
