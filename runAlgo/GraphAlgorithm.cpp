#include "GraphAlgorithm.h"

#include "Graph.h"

GraphAlgorithm::GraphAlgorithm() : TableAlgorithm("graph") {}

std::unique_ptr<TableAlgorithm> GraphAlgorithm::factory::make()
{
	return std::unique_ptr<GraphAlgorithm>(new GraphAlgorithm());
}

Solution GraphAlgorithm::solve(const Problem &problem, solve_cb_t callback)
{
	Solution s;

	// Make a graph of the companies that cannot be together
	Graph<company_id_t> g;
	for (auto pair : problem.separate) {
		g.connect(pair.first, pair.second);
	}

	// Use graph coloring to separate them into tables
	auto opt_col = g.color(problem.n_tables);
	if (!opt_col.first) {
		// Return empty solution because we could not satisfy
		// separate companies constraint
		std::cout << "ERROR_COLORS" << std::endl;
		return s;
	}

	// TODO use the colored companies to start tables
	std::vector<std::unordered_set<company_id_t>> &tables;
	
	
	callback(s);
	return s;
}
