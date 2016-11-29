//
// Created by Benjamin on 2016-11-29.
//

#ifndef INF4705_TP3_TABLEANDCOMPANIES_H
#define INF4705_TP3_TABLEANDCOMPANIES_H

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
    float weight =0.0f;
};


#endif //INF4705_TP3_TABLEANDCOMPANIES_H
