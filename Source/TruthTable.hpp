//
//  TruthTable.hpp
//  Inference Engine
//
//  Created by Matthew Rimkevicus on 01/05/2016
//

#pragma once

#include "InferenceEngine.hpp"
#include "KnowledgeBase.hpp"
#include <list>
#include <map>
#include "Evaluator.hpp"

class TruthTable : public InferenceEngine
{
private:
	const KnowledgeBase& kb;
	vector<map<Token, bool>> tokenTable;
	vector<map<Clause, bool>> clauseTable;
	set<Token> alwaysTrueTokens;

	string hazard = "breeze";
	const bool wumpusMode;

	void findAlwaysTrueTokens();
	void prepTruthTableForNextIndexColumn();
	void buildTruthTable();
	void updateClauseTable(Clause& clause);

public:
	TruthTable(const KnowledgeBase& kb, const bool wumpusMode = false);

	bool clauseResultInRow(const Clause& clause, map<Token, bool>& row);

	friend ostream& operator<<(ostream& output, const TruthTable& TT);

	virtual bool ask(const Token& id, const set<Token>& assumptions = {});
};