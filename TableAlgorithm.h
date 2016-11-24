#include <chrono>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

typedef int company_id_t;

class Problem {
public:
	int n_tables;
	std::vector<size_t> companies;
	std::multimap<company_id_t, company_id_t> separate;
	std::multimap<company_id_t, company_id_t> want_separate;
	std::multimap<company_id_t, company_id_t> want_together;

	static std::unique_ptr<Problem> fromStream(std::istream &in);
};

class Solution {
public:
	std::vector< std::vector<company_id_t> > tables;
};

std::ostream &operator<<(std::ostream &strm, const Solution &s);

typedef std::function<void(const Solution&,
			   const std::chrono::duration<float>&)>
	run_cb_t;

class TableAlgorithm {
	typedef std::chrono::high_resolution_clock clock_t;
	std::chrono::time_point<clock_t> start;
	std::string name;
protected:
	TableAlgorithm(std::string name);
	
	typedef std::function<void(const Solution &s)> solve_cb_t;
	virtual Solution *solve(const Problem &problem,
				solve_cb_t callback) = 0;
public:
	std::string getName();
	Solution *run(const Problem &problem, run_cb_t callback);

	class factory {
	public:
		virtual std::unique_ptr<TableAlgorithm> make() = 0;
	};

	static const std::vector<TableAlgorithm::factory*> algorithms;
	
	static std::vector<std::string> list();
	static std::unique_ptr<TableAlgorithm> make(std::string algoname);
};

class TrivialAlgo : public TableAlgorithm {
	virtual Solution *solve(const Problem &problem,
				solve_cb_t callback);
public:
	TrivialAlgo();
	class factory : public TableAlgorithm::factory {
		virtual std::unique_ptr<TableAlgorithm> make();
	};
};
