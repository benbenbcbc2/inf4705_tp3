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
	for (unsigned int i = 0; i < problem.companies.size(); ++i) {
		g.add_node(i, problem.companies[i]);
	}
	for (auto pair : problem.separate) {
		g.connect(pair.first, pair.second);
	}
	for (auto pair : problem.want_separate) {
		g.connect(pair.first, pair.second, Set::WSP);
	}
	for (auto pair : problem.want_together) {
		g.connect(pair.first, pair.second, Set::WTG);
	}

	try {
		auto cb = [&s,callback]
			(std::vector<NodeGroup<company_id_t>>& vec){
			s.tables.clear();
			for (auto &g : vec) {
				s.tables.emplace_back(g.ids.begin(),
						      g.ids.end());
			}
			callback(s);
		};
		g.place(float(problem.n_people)/problem.n_tables,
			problem.n_tables,
			cb);
	} catch (GroupExcept &c) {
		// Return empty solution because we could not satisfy
		// separate companies constraint
		std::cout << "ERROR : " << c.what() << std::endl;
	}

	return s;
}
