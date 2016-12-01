#ifndef AlgoCotton2_H
#define AlgoCotton2_H

#include "TableAlgorithm.h"
#include "TableAndCompanies.h"

class AlgoCotton2 : public TableAlgorithm {
	virtual Solution solve(const Problem &problem,
		solve_cb_t callback);
	void setCompanies(const Problem &problem, std::map<company_id_t, Company*>& companies);
	bool placeTheMandatories(std::vector<TableWithVectors>& tables, std::map<company_id_t, Company*>& companies);
	void recursiveSolving(company_id_t companyToAdd, size_t tableToAddTo, size_t nextTableToPlaceTo, std::vector<TableWithVectors> tables, std::map<company_id_t, Company*> companiesToPlace, size_t& averagePerTable, solve_cb_t& callback);
	void vecBoolToVecInt(std::vector<bool>& vecBool, std::vector<int>& vecInt);
	void IntersectionVecIntAndVecBool(std::vector<int>& vecInt, std::vector<bool>& vecBool);

public:
	AlgoCotton2();
	~AlgoCotton2();
	class factory : public TableAlgorithm::factory {
		virtual std::unique_ptr<TableAlgorithm> make();
	};
	Solution bestSol;
    float bestQuality;
	std::vector<Company*> companies;
};

#endif // AlgoCotton2_H