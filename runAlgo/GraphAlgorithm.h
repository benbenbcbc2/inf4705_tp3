#ifndef _GRAPHALGORITHM_H_
#define _GRAPHALGORITHM_H_

#include <TableAlgorithm.h>

class GraphAlgorithm : public TableAlgorithm {
	virtual Solution solve(const Problem &problem,
			       solve_cb_t callback);
public:
	GraphAlgorithm();
	class factory : public TableAlgorithm::factory {
		virtual std::unique_ptr<TableAlgorithm> make();
	};
};

#endif // _GRAPHALGORITHM_H_
