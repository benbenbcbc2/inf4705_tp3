//
// Created by Benjamin on 2016-11-29.
//

#include "TableAndCompanies.h"
#include <algorithm>    // std::find

void Table::addCompany(Company* company, int weight)
{
    companies.push_back(company->id);
    employees += company->employees;
    this->weight += weight;
    vecUnion(separate, company->separate);
    for (auto& companyNb : company->want_separate)
    {
        if (want_separate.find(companyNb) != want_separate.end())
        {
            want_separate[companyNb] += 1;
        }
        else if (want_together.find(companyNb) != want_together.end())
        {
            want_together[companyNb] -= 1;
            if (want_together[companyNb] == 0)
            {
                want_together.erase(companyNb);
            }
        }
        else
        {
            want_separate[companyNb] = 1;
        }
    }
    for (auto& companyNb : company->want_together)
    {
        if (want_together.find(companyNb) != want_together.end())
        {
            want_together[companyNb] += 1;
        }
        else if (want_separate.find(companyNb) != want_separate.end())
        {
            want_separate[companyNb] -= 1;
            if (want_separate[companyNb] == 0)
            {
                want_separate.erase(companyNb);
            }
        }
        else
        {
            want_together[companyNb] = 1;
        }
    }
    vecDifference(want_together, separate);
    vecDifference(want_separate, separate);
}

void Table::vecDifference(std::map<company_id_t, size_t>& a, std::vector<company_id_t>& b)
{
    for (company_id_t& companyNb : b)
    {
        a.erase(companyNb);
    }
}

void Table::vecDifference(std::vector<company_id_t>& a, std::map<company_id_t, size_t>& b)
{
    for (auto& companyNb : b)
    {
        auto it = std::find(a.begin(), a.end(), companyNb.first);
        if (it != a.end())
            a.erase(it);
    }
}

void Table::vecDifference(std::vector<company_id_t>& a, std::vector<company_id_t>& b)
{
    for (company_id_t& companyNb : b)
    {
        auto it = std::find(a.begin(), a.end(), companyNb);
        if (it != a.end())
            a.erase(it);
    }
}

void Table::vecUnion(std::map<company_id_t, size_t>& a, std::vector<company_id_t>& b)
{
    for (company_id_t& companyNb : b)
    {
        if (a.find(companyNb) != a.end())
        {
            a[companyNb] += 1;
        }
        else
        {
            a[companyNb] = 1;
        }
    }
}

void Table::vecUnion(std::vector<company_id_t>& a, std::vector<company_id_t>& b)
{
    std::vector<company_id_t> temp;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(temp));
    a = temp;
}

void Table::vecIntersection(std::vector<company_id_t>& a, std::vector<company_id_t>& b)
{
    std::vector<company_id_t> temp;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(temp));
    a = temp;
}

Company::Company(company_id_t id, size_t employees)
{
    this->id = id;
    this->employees = employees;
}