#include <algorithm>

#include "TableAlgorithm.h"

Problem Problem::fromStream(std::istream& in)
{
	Problem p;
	int n_companies;
	auto maps = {&p.separate,
		     &p.want_together,
		     &p.want_together};
	in >> p.n_tables;
	in >> n_companies;
	p.companies.resize(n_companies);
	for (int i=0; i<n_companies; ++i)
		in >> p.companies[i];
	for (auto map : maps) {
		int n_assoc;
		in >> n_assoc;
		for (int i=0; i<n_assoc; ++i) {
			company_id_t a, b;
			in >> a >> b;
			map->insert(std::make_pair(a, b));
			map->insert(std::make_pair(b, a));
		}
	}
	return p;
}

std::ostream& operator<<(std::ostream &strm, const Solution &s)
{
	for (auto t: s.tables) {
		bool first = true;
		for (company_id_t i: t){
			if (first)
				first = false;
			else
				strm << " ";
			strm << i;
		}
		strm << std::endl;
	}
	strm << "fin" << std::endl;
	return strm;
}

TableAlgorithm::TableAlgorithm(std::string name_) : name(name_) {}

std::string TableAlgorithm::getName()
{
	return this->name;
}

std::vector<TableAlgorithm::factory*> TableAlgorithm::algorithms;

std::vector<std::string> TableAlgorithm::list()
{
	std::vector<std::string> names;
	for (auto *factory: TableAlgorithm::algorithms)
		names.push_back(factory->make()->getName());
	return names;
}

std::unique_ptr<TableAlgorithm> TableAlgorithm::make(std::string algoname)
{
	auto fact = std::find_if(
		TableAlgorithm::algorithms.begin(),
		TableAlgorithm::algorithms.end(),
		[algoname](TableAlgorithm::factory* f){
			return f->make()->getName() == algoname;
		});
	if (fact != TableAlgorithm::algorithms.end())
		return (*fact)->make();
	return std::unique_ptr<TableAlgorithm>(nullptr);
}

void TableAlgorithm::addAlgorithm(factory *fact)
{
	TableAlgorithm::algorithms.push_back(fact);
}

std::unique_ptr<TableAlgorithm> makeDefault()
{
	try {
		return TableAlgorithm::algorithms.at(0)->make();
	} catch (std::out_of_range e) {
		return std::unique_ptr<TableAlgorithm>(nullptr);
	}
}

Solution TableAlgorithm::run(const Problem &problem, run_cb_t callback)
{
	this->start = clock_t::now();
	return this->solve(problem, [this, callback](const Solution &s){
			callback(s, clock_t::now()-this->start);
		});
}
