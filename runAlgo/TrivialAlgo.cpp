#include "TrivialAlgo.h"

TrivialAlgo::TrivialAlgo() : TableAlgorithm("triv") {}

std::unique_ptr<TableAlgorithm> TrivialAlgo::factory::make()
{
	return std::unique_ptr<TrivialAlgo>(new TrivialAlgo());
}

Solution TrivialAlgo::solve(const Problem &problem, solve_cb_t callback)
{
	Solution s;
	std::vector<company_id_t> v;
	v.push_back(5);
	v.push_back(6);
	std::vector< std::vector<company_id_t> > test;
	test.push_back(v);
	v.push_back(7);
	v.push_back(1);
	test.push_back(v);
	s.tables = test;
	callback(s);
	return s;
}
