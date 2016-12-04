#include <algorithm>
#include <cmath>
#include <sstream>
#include <unordered_set>
#include <numeric>

#include "TableAlgorithm.h"

float average(std::vector<unsigned int> list)
{
	return float(std::accumulate(list.begin(), list.end(), 0))
		/ list.size();
}

float std_dev(std::vector<unsigned int> list)
{
	float avg = average(list);
	float std_dev2 = 0;
	for (auto i : list) {
		std_dev2 += std::pow(i - avg, 2);
	}
    std_dev2 /=list.size();
	return std::sqrt(std_dev2);
}

Problem Problem::fromStream(std::istream& in)
{
	Problem p;
	int n_companies;
	auto maps = {&p.separate,
		     &p.want_together,
		     &p.want_separate};
	in >> p.n_tables;
	in >> n_companies;
	p.companies.resize(n_companies);
	p.n_people = 0;
	for (int i = 0; i < n_companies; ++i) {
		in >> p.companies[i];
		p.n_people += p.companies[i];
	}
	for (auto map : maps) {
		int n_assoc;
		in >> n_assoc;
		for (int i=0; i<n_assoc; ++i) {
			company_id_t a, b;
			in >> a >> b;
			map->insert(std::make_pair(a, b));
		}
	}
	return p;
}

float Problem::eval(Solution &s)
{
	// Check that all companies are seated only once
	std::vector<bool> seated(this->companies.size(), false);
	for (auto table: s.tables) {
		for (company_id_t id : table) {
			try {
				if (!seated.at(id))
					seated[id] = true;
				else
					return INFINITY;
			} catch (std::out_of_range &out) {
				return INFINITY;
			}
		}
	}
	for (auto s : seated) {
		if (!s)
			return INFINITY;
	}
	
	// Calculate cost and check for companies that want to be separate
	float cost = 0;
	std::vector<unsigned int> table_sizes;
	for (auto table : s.tables) {
		unsigned int table_size = 0;
		std::unordered_set<company_id_t> set(
			table.begin(), table.end());
		for (company_id_t id : table) {
			table_size += this->companies.at(id);
			std::initializer_list<std::pair<std::multimap<int, int>*, std::function<float(float)>>> pair_list_actions = {
				std::make_pair(
					&this->separate,
					[](float cost){return INFINITY;}),
				std::make_pair(
					&this->want_separate,
					[](float cost){return cost+1;}),
				std::make_pair(
					&this->want_together,
					[](float cost){return cost-1;}),
			};
			for (auto list : pair_list_actions) {
				for (auto it = list.first->
					     lower_bound(id);
				     it != list.first->upper_bound(id);
				     ++it) {
					if (set.find(it->second) != set.end()) {
						auto ret =
							list.second(cost);
						if (std::isinf(ret))
							return ret;
						else cost = ret;
					}
				}
			}
		}
		table_sizes.push_back(table_size);
	}
	cost += std_dev(table_sizes);
	return cost;
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

std::istream &operator>>(std::istream &strm, Solution& s)
{
	s.tables.clear();
	for (std::string line;
	     std::getline(strm, line), line != "fin";) {
		std::vector<company_id_t> table;
        if(line.empty()) {
            s.tables.push_back(table);
        }
        else {
            std::istringstream in(line);
            in.exceptions(std::ifstream::failbit);
            for (company_id_t id;;) {
                if (!in.good())
                    break;
                in >> id;
                table.push_back(id);
            }
            if (table.empty())
                strm.setstate(std::istream::failbit);
            s.tables.push_back(table);
        }
	}
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
