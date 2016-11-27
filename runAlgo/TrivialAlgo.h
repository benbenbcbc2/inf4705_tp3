#ifndef _TRIVIALALGO_H_
#define _TRIVIALALGO_H_

#include <TableAlgorithm.h>

class TrivialAlgo : public TableAlgorithm {
	virtual Solution solve(const Problem &problem,
			       solve_cb_t callback);
public:
	TrivialAlgo();
	class factory : public TableAlgorithm::factory {
		virtual std::unique_ptr<TableAlgorithm> make();
	};
};

#endif // _TRIVIALALGO_H_
