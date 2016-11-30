#include "AlgoCotton2.h"
#include <algorithm>    // std::sort
#include <iterator>		// std::back_inserter
#include <climits>		// INT_MAX
#include <math.h>		// pow

AlgoCotton2::AlgoCotton2() : TableAlgorithm("cotton2") 
{
	bestQuality = INT_MAX;
}

AlgoCotton2::~AlgoCotton2()
{
	for (auto company : companies)
	{
		delete company;
	}
}

std::unique_ptr<TableAlgorithm> AlgoCotton2::factory::make()
{
	return std::unique_ptr<AlgoCotton2>(new AlgoCotton2());
}

Solution AlgoCotton2::solve(const Problem &problem, solve_cb_t callback)
{
    size_t nbCompanies = problem.companies.size();
    std::map<company_id_t, Company*> companiesToPlace;
    std::vector<TableWithVectors> tables(problem.n_tables, TableWithVectors(nbCompanies));
	setCompanies(problem, companiesToPlace);
	if (nbCompanies > 0)
	{
		size_t average = problem.n_people / problem.n_tables;
		recursiveSolving(0, 0, 1, tables, companiesToPlace, average, callback);
		std::cout << "FINI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
		callback(bestSol);
		return bestSol;
	}
	else
	{
		return Solution();
	}
}


void AlgoCotton2::setCompanies(const Problem &problem, std::map<company_id_t, Company*>& companies)
{
	Company* companyToPlace;
	for (size_t i = 0; i < problem.companies.size(); i++)
	{
		companyToPlace = new Company(i, problem.companies[i]);
		companies.insert(std::pair<company_id_t, Company*>(i, companyToPlace));
		this->companies.push_back(companyToPlace);
	}
	for (auto separate : problem.separate)
	{
        companies[separate.first]->separate.push_back(separate.second);
	}
	for (auto want_separate : problem.want_separate)
	{
		companies[want_separate.first]->want_separate.push_back(want_separate.second);
	}
	for (auto want_together : problem.want_together)
	{
		companies[want_together.first]->want_together.push_back(want_together.second);
	}
}

bool AlgoCotton2::placeTheMandatories(std::vector<TableWithVectors>& tables, std::map<company_id_t, Company*>& companies)
{
	std::vector <company_id_t> companiesOK;
	TableWithVectors* table;
	bool continueToPlace = true;
	while (continueToPlace)
	{
		continueToPlace = false;
		for (size_t i = 0; i < tables.size(); i++)
		{
			if (i == 0)
			{
				vecBoolToVecInt(tables[1].separate, companiesOK);
			}
			else
			{
				vecBoolToVecInt(tables[0].separate, companiesOK);
			}
			if (companiesOK.empty())
				break;
			for (size_t j = 0; j < tables.size(); j++)
			{
				if (i != j)
					IntersectionVecIntAndVecBool(companiesOK, tables[j].separate);
				if (companiesOK.empty())
					break;
			}
			// Test if there is a number that can't go in any table
			std::vector <company_id_t> companiesThatCantGoAnywhere = companiesOK;
			IntersectionVecIntAndVecBool(companiesOK, tables[i].separate);
			if(!companiesThatCantGoAnywhere.empty())
			{
				return false;
			}
			if (!companiesOK.empty())
			{
				table = &tables[i];
				for (size_t j :companiesOK)
				{
					if (companies.find(j) != companies.end())
					{
						continueToPlace = true;
						table->addCompany(companies[j]);
						companies.erase(j);
					}
				}
			}
		}
	}
	return true;
}

void AlgoCotton2::recursiveSolving(company_id_t companyToAdd, size_t tableToAddTo, size_t nextTableToPlaceTo, std::vector<TableWithVectors> tables, std::map<company_id_t, Company*> companiesToPlace, size_t& averagePerTable, solve_cb_t& callback)
{
	TableWithVectors& table = tables[tableToAddTo];
	table.addCompany(companiesToPlace[companyToAdd]);
	companiesToPlace.erase(companyToAdd);
	if(!placeTheMandatories(tables, companiesToPlace))
	{
		return;
	}

	if (companiesToPlace.empty())
	{
        Solution sol;
		float quality = 0.0f;
		float S1 = 0.0f;
		float S2 = 0.0f;

		for (auto& table : tables)
		{
			sol.tables.push_back(table.companies);
			quality += table.weight;
			S1 += table.employees;
			S2 += pow(table.employees, 2);
		}
		float average = S1 / tables.size();
		float variance = S2 / tables.size() - pow(average,2);
		float deviation = sqrt(variance);
		quality += deviation;
		if (quality < bestQuality)
		{
			bestSol = sol;
            bestQuality = quality;
			std::cout << "QUALITY : " << quality << std::endl << std::flush;
			callback(sol);
		}
	}
	else
	{
		company_id_t nextCompanyToAdd = companiesToPlace.begin()->first;
		size_t tableNb;
		//size_t companyEmployees = companiesToPlace.begin()->second->employees;
		for (size_t i = 0; i < tables.size(); i++)
		{
			tableNb = (nextTableToPlaceTo+i)%tables.size();
			if(!tables[tableNb].separate[nextCompanyToAdd] && tables[tableNb].want_separate[nextCompanyToAdd] < 0)
			{
				recursiveSolving(nextCompanyToAdd, tableNb, (tableNb+1)%tables.size(), tables, companiesToPlace, averagePerTable, callback);
			}
		}
		for (size_t i = 0; i < tables.size(); i++)
		{
			tableNb = (nextTableToPlaceTo+i)%tables.size();
			if(!tables[tableNb].separate[nextCompanyToAdd] && tables[tableNb].want_separate[nextCompanyToAdd]  == 0)
			{
				recursiveSolving(nextCompanyToAdd, tableNb, (tableNb+1)%tables.size(), tables, companiesToPlace, averagePerTable, callback);
			}
		}
		for (size_t i = 0; i < tables.size(); i++)
		{
			tableNb = (nextTableToPlaceTo+i)%tables.size();
			if(!tables[tableNb].separate[nextCompanyToAdd] && tables[tableNb].want_separate[nextCompanyToAdd] > 0)
			{
				recursiveSolving(nextCompanyToAdd, tableNb, (tableNb+1)%tables.size(), tables, companiesToPlace, averagePerTable, callback);
			}
		}
	}
}

void AlgoCotton2::vecBoolToVecInt(std::vector<bool>& vecBool, std::vector<int>& vecInt)
{
    for (size_t i = 0; i < vecBool.size(); i++)
    {
        if(vecBool[i])
        {
			vecInt.push_back(i);
        }
    }
}

std::vector<int>& AlgoCotton2::IntersectionVecIntAndVecBool(std::vector<int>& vecInt, std::vector<bool>& vecBool)
{
    std::vector<int> temp;
    for(auto it = vecInt.begin(); it != vecInt.end();it++)
    {
        if (vecBool[*it])
        {
            temp.push_back(*it);
        }
    }
	vecInt = temp;
    return vecInt;
}