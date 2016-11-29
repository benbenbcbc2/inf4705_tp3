#ifndef AlgoCotton1_H
#define AlgoCotton1_H

#include "TableAlgorithm.h"

class Company {
public:
	Company(company_id_t id, size_t employees);
	std::vector<company_id_t> separate;
	std::vector<company_id_t> want_separate;
	std::vector<company_id_t> want_together;
	company_id_t id;
	size_t employees;
};

class Table {
public:
	static void vecDifference(std::map <company_id_t, size_t>& a, std::vector<company_id_t>& b);
	static void vecDifference(std::vector<company_id_t>& a, std::map<company_id_t, size_t>& b);
	static void vecDifference(std::vector<company_id_t>& a, std::vector<company_id_t>& b);
	static void vecUnion(std::map <company_id_t, size_t>& a, std::vector<company_id_t>& b);
	static void vecUnion(std::vector<company_id_t>& a, std::vector<company_id_t>& b);
	static void vecIntersection(std::vector<company_id_t>& a, std::vector<company_id_t>& b);
	void addCompany(Company* company, int weight);
	size_t employees;
	std::vector<company_id_t> companies;
	std::vector<company_id_t> separate;
	std::map <company_id_t, size_t> want_separate;
	std::map <company_id_t, size_t> want_together;
	int weight = 0;
};

class AlgoCotton1 : public TableAlgorithm {
	virtual Solution solve(const Problem &problem,
		solve_cb_t callback);
	void setProblem(const Problem &problem, std::map<company_id_t, Company*>& companiesToPlace, std::vector<Table>& tables);
	void placeTheMandatories(size_t& tablesWithPeople, size_t& fullTables, std::vector<Table>& tables, std::map<company_id_t, Company*>& companiesToPlace, size_t& peopleToPlace);
	void recursiveSolving(size_t tablesWithPeople, size_t fullTables, size_t companyToAdd, size_t tableToAddTo, std::vector<Table> tables, std::map<company_id_t, Company*> companiesToPlace, int weight, size_t peopleToPlace, size_t& averagePerTable, solve_cb_t& callback);
public:
	AlgoCotton1();
	class factory : public TableAlgorithm::factory {
		virtual std::unique_ptr<TableAlgorithm> make();
	};
	static Solution bestSol;
};

#endif // AlgoCotton1_H