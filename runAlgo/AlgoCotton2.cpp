#include "AlgoCotton2.h"
#include <algorithm>    // std::sort
#include <iterator>		// std::back_inserter
#include <climits>		// INT_MAX
#include <math.h>		// pow

Solution AlgoCotton2::bestSol;

AlgoCotton2::AlgoCotton2() : TableAlgorithm("cotton2") 
{
	bestSol.quality = INT_MAX;
}

std::unique_ptr<TableAlgorithm> AlgoCotton2::factory::make()
{
	return std::unique_ptr<AlgoCotton2>(new AlgoCotton2());
}

Solution AlgoCotton2::solve(const Problem &problem, solve_cb_t callback)
{
	std::map<company_id_t, Company*> companiesToPlace;
	std::vector<Table> tables(problem.n_tables);
	setProblem(problem, companiesToPlace, tables);
	if (!companiesToPlace.empty())
	{
		size_t peopleToPlace = problem.n_people;
		Table& table = tables[0];
		peopleToPlace -= companiesToPlace[0]->employees;
		table.addCompany(companiesToPlace[0], 0);
		companiesToPlace.erase(0);
		size_t tablesWithPeople = 1;
		size_t fullTables = 0;
		placeTheMandatories(tablesWithPeople, fullTables, tables, companiesToPlace, peopleToPlace);
		size_t average = peopleToPlace / problem.n_tables;
		recursiveSolving(tablesWithPeople, fullTables, 0, 0, tables, companiesToPlace, 0, peopleToPlace, average, callback);
		for (auto company : companiesToPlace)
		{
			delete company.second;
		}
		std::cout << "FINI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
		callback(bestSol);
		return bestSol;
	}
	else
	{
		return Solution();
	}
}


void AlgoCotton2::setProblem(const Problem &problem, std::map<company_id_t, Company*>& companiesToPlace, std::vector<Table>& tables)
{
	std::fill(tables.begin(), tables.end(), Table());
	for (size_t i = 0; i < problem.companies.size(); i++)
	{
		companiesToPlace.insert(std::pair<company_id_t, Company*>(i, new Company(i, problem.companies[i])));
	}
	for (auto& separate : problem.separate)
	{
		companiesToPlace[separate.first]->separate.push_back(separate.second);
	}
	for (auto& want_separate : problem.want_separate)
	{
		companiesToPlace[want_separate.first]->want_separate.push_back(want_separate.second);
	}
	for (auto& want_together : problem.want_together)
	{
		companiesToPlace[want_together.first]->want_together.push_back(want_together.second);
	}
}

void AlgoCotton2::placeTheMandatories(size_t& tablesWithPeople, size_t& fullTables, std::vector<Table>& tables, std::map<company_id_t, Company*>& companiesToPlace, size_t& peopleToPlace)
{
	std::vector <company_id_t> companiesOK;
	Table* table;
	for (size_t i = tablesWithPeople; i < tables.size(); i++)
	{
		table = &tables[i];
		companiesOK = tables[0].separate;
		if (companiesOK.empty())
		{
			return;
		}
		for (size_t j = 1; j < i; j++)
		{
			Table::vecIntersection(companiesOK, tables[j].separate);
			if (companiesOK.empty())
			{
				return;
			}	
		}
		for (size_t j : companiesOK)
		{
			if (companiesToPlace.find(j) != companiesToPlace.end())
			{
				peopleToPlace -= companiesToPlace[j]->employees;
				table->addCompany(companiesToPlace[j], 0);
				companiesToPlace.erase(j);
				tablesWithPeople++;
				break;
			}
		}
	}
	bool continueToPlace = true;
	while (continueToPlace)
	{
		continueToPlace = false;
		for (size_t i = 0; i < tables.size(); i++)
		{
			table = &tables[i];
			if (i == 0)
			{
				companiesOK = tables[1].separate;
			}
			else
			{
				companiesOK = tables[0].separate;
			}
			if (companiesOK.empty())
				break;
			for (size_t j = 0; j < tables.size(); j++)
			{
				if (i != j)
					Table::vecIntersection(companiesOK, tables[j].separate);
				if (companiesOK.empty())
					break;
			}
			if (!companiesOK.empty())
			{
				for (size_t j :companiesOK)
				{
					if (companiesToPlace.find(j) != companiesToPlace.end())
					{
						continueToPlace = true;
						peopleToPlace -= companiesToPlace[j]->employees;
						if (table->want_separate.find(j) != table->want_separate.end())
						{
							table->addCompany(companiesToPlace[j], table->want_separate[j]);
						}
						else if (table->want_together.find(j) != table->want_together.end())
						{
							table->addCompany(companiesToPlace[j], -table->want_together[j]);
						}
						else
						{
							table->addCompany(companiesToPlace[j], 0);
						}
						companiesToPlace.erase(j);
					}
				}
			}
		}
	}
}

void AlgoCotton2::recursiveSolving(size_t tablesWithPeople, size_t fullTables, size_t companyToAdd, size_t tableToAddTo, std::vector<Table> tables, std::map<company_id_t, Company*> companiesToPlace, int weight, size_t peopleToPlace, size_t& averagePerTable, solve_cb_t& callback)
{
	Solution sol;
	Table& table = tables[tableToAddTo];
	if (companyToAdd != 0)
	{
		peopleToPlace -= companiesToPlace[companyToAdd]->employees;
		table.addCompany(companiesToPlace[companyToAdd], weight);
		companiesToPlace.erase(companyToAdd);
	}
	if (tableToAddTo == tablesWithPeople)
	{
		tablesWithPeople++;
	}
	placeTheMandatories(tablesWithPeople, fullTables, tables, companiesToPlace, peopleToPlace);
	if (companiesToPlace.empty())
	{
		sol.quality = 0.0f;
		float S1 = 0.0f;
		float S2 = 0.0f;

		for (auto& table : tables)
		{
			sol.tables.push_back(table.companies);
			sol.quality += table.weight;
			S1 += table.employees;
			S2 += pow(table.employees, 2);
		}
		float average = S1 / tables.size();
		float variance = S2 / tables.size() - pow(average,2);
		float deviation = sqrt(variance);
		sol.quality += deviation;
		if (sol.quality < bestSol.quality)
		{
			bestSol = sol;
			std::cout << "QUALITY : " << sol.quality << std::endl << std::flush;
			callback(sol);
		}
	}
	else
	{
		if (table.employees >= averagePerTable - 2 && table.employees <= averagePerTable + 2 && tableToAddTo != tables.size() - 1)
		{
			size_t newAverage = peopleToPlace / (tables.size() - tableToAddTo - 1);
			recursiveSolving(tablesWithPeople, fullTables + 1, companiesToPlace.begin()->first, tableToAddTo + 1, tables, companiesToPlace, 0, peopleToPlace, newAverage, callback);
		}
		if (table.employees < averagePerTable + 2 || tableToAddTo == tables.size() - 1)
		{
			for (auto& want_together : table.want_together)
			{
				if (companiesToPlace.find(want_together.first) != companiesToPlace.end())
				{
					recursiveSolving(tablesWithPeople, fullTables, want_together.first, tableToAddTo, tables, companiesToPlace, -want_together.second, peopleToPlace, averagePerTable, callback);
				}
			}
			std::vector < company_id_t> companiesOK;
			for (auto it = companiesToPlace.begin(); it != companiesToPlace.end(); ++it)
			{
				companiesOK.push_back(it->first);
			}
			Table::vecDifference(companiesOK, table.want_separate);
			Table::vecDifference(companiesOK, table.separate);
			for (auto& company : companiesOK)
			{
				recursiveSolving(tablesWithPeople, fullTables, company, tableToAddTo, tables, companiesToPlace, 0, peopleToPlace, averagePerTable, callback);
			}
			for (auto& want_separate : table.want_separate)
			{
				if (companiesToPlace.find(want_separate.first) != companiesToPlace.end())
				{
					recursiveSolving(tablesWithPeople, fullTables, want_separate.first, tableToAddTo, tables, companiesToPlace, want_separate.second, peopleToPlace, averagePerTable, callback);
				}
			}
		}
	}
}