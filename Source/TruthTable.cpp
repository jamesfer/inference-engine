//
//  TruthTable.cpp
//  Inference Engine
//
//  Created by Matthew Rimkevicus on 01/05/2016
//

#include "TruthTable.hpp"
#include <iomanip>

using namespace std;

TruthTable::TruthTable(const KnowledgeBase& kb, const bool wumpusMode)
	: kb(kb), wumpusMode(wumpusMode)
{
	findAlwaysTrueTokens();
	buildTruthTable();
}

void TruthTable::findAlwaysTrueTokens()
{
	for (const Clause& clause : kb.getClauses())
	{
		if (clause.size() == 1)
			alwaysTrueTokens.insert(clause.getTokens().front());
	}
}

void TruthTable::prepTruthTableForNextIndexColumn()
{
	if (!tokenTable.empty())
	{
		size_t tableSize = tokenTable.size();
		for(size_t i = 0; i < tableSize; i++)
		{
			tokenTable.push_back(tokenTable[i]);
		}
	}
	else
	{
		// Add 2 rows
		tokenTable.push_back(map<Token, bool>());
		tokenTable.push_back(map<Token, bool>());
	}
}

void TruthTable::buildTruthTable()
{
	for (Token token : kb.getTokens())
	{
		if ((token.getValue().find(hazard) != string::npos) ||
			(alwaysTrueTokens.size() && !alwaysTrueTokens.count(token))) // count only returns 0 or 1 because sets are unique
		{
			if (wumpusMode)
				token = Token(token.getValue().substr(token.getValue().size() - 2, 2));
			prepTruthTableForNextIndexColumn();

			for (unsigned int i = 0; i < tokenTable.size(); i++)
			{
				if (i < (tokenTable.size() / 2))
					tokenTable[i].insert(pair<Token, bool>(token, false));
				else
					tokenTable[i].insert(pair<Token, bool>(token, true));
			}
		}
	}
	for (const Token& token : alwaysTrueTokens)
	{
		for (unsigned int i = 0; i < tokenTable.size(); i++)
			tokenTable[i].insert(pair<Token, bool>(token, true));
	}
	// start filling the clause table
	clauseTable.resize(tokenTable.size());
	for (Clause clause : kb.getClauses())
	{
		if (wumpusMode)
		{
			if (clause.getTokens().front().getValue().find(hazard) != string::npos)
			{
				clause = clause.getAntecedent();	// remove the consequent
				// remove the hazard string from each token
				Clause tempClause;
				for (Token token : clause.getTokens())
				{
					if (!token.isOperator())
						token = token.getValue().substr(token.getValue().size() - 2, 2);
					tempClause << token;
				}
				updateClauseTable(tempClause);
			}
		}
		else
		{
			if (clause.size() > 1)		// ignore clauses stating always true things
				updateClauseTable(clause);
		}
	}
}

void TruthTable::updateClauseTable(Clause& clause)
{
	for (unsigned int i = 0; i < clauseTable.size(); i++)
	{
		Evaluator evaluator(tokenTable[i]);
		clauseTable[i].insert(pair<Clause, bool>(clause, evaluator(clause)));
	}
}

ostream& operator<<(ostream& output, const TruthTable& TT)
{
	// output the clause substitute names
	{
		int clauseNumber = 1;
		for (const pair<Clause, bool>& clause : TT.clauseTable[0])
		{
			output << "C" << clauseNumber++ << ": " << clause.first << endl;
		}
	}

	unsigned int numberOfYes = 0;
	output << "|";
	for (const pair<Token, bool>& token : TT.tokenTable[0])
	{
		output << setw(2) << token.first.getValue() << " |";
	}
	output << "|"; // double line between tokens and clauses
	for (unsigned int i = 1; i <= TT.clauseTable[0].size(); i++)
	{
		output << "C" << setw(2) << left << i << "|";
	}
	output << endl;

	// output the table
	for (unsigned int i = 0; i < TT.tokenTable.size(); i++)
	{
		output << "|";
		for (const pair<Token, bool>& token : TT.tokenTable[i])
		{
			if (token.second)
				output << setw(3) << left << "T";
			else
				output << setw(3) << right << "F";
			output << "|";
		}
		output << "|"; // double line between tokens and clauses

		bool allClausesTrue = true;
		for (const pair<Clause, bool>& clause : TT.clauseTable[i])
		{
			if (clause.second)
				output << setw(3) << left << "T";
			else
			{
				output << setw(3) << right << "F";
				allClausesTrue = false;
			}
			output << "|";
		}
		if (allClausesTrue)
		{
			numberOfYes++;
			output << " <-";
		}
		output << endl;
	}

	if (numberOfYes) // > 0
		output << endl << "YES: " << numberOfYes << endl;
	else
		output << endl << "NO" << endl;

	return output;
}

bool TruthTable::clauseResultInRow(const Clause& clause, map<Token, bool>& row)
{
	if (row.size() != tokenTable.front().size())
	{
		// fill in the missing tokens as false
		for (int i = 0; (i < 2) && (row.size() != tokenTable.front().size()); i++)
		{
			for (int j = 0; (j < 2) && (row.size() != tokenTable.front().size()); j++)
			{
				bool inRow = false;
				string s = "";
				s += (char)(i + '0');
				s += (char)(j + '0');
				for (const pair<Token, bool>& token : row)
				{
					if (token.first.getValue() == s)
					{
						inRow = true;
						break;
					}
				}
				if (!inRow)
					row.insert(pair<Token, bool>(Token(s), false));
			}
		}
	}
	size_t rowNumber;
	for (rowNumber = 0; rowNumber < tokenTable.size(); rowNumber++)
	{
		bool matchingRow = true;
		for (pair<Token, bool> token : row)
		{
			if (row.at(token.first) != tokenTable[rowNumber].at(token.first))
			{
				matchingRow = false;
				break;
			}
		}
		if (matchingRow) break;
	}
	return clauseTable[rowNumber].at(clause);
}

bool TruthTable::ask(const Token& id, const set<Token>& assumptions)
{
	string trap = id.getValue().substr(0, id.getValue().size() - 2);
	if (trap == "pit")
		hazard = "breeze";
	else if (trap == "wumpus")
		hazard = "stench";
	else
		throw runtime_error("Invalid ask token.");
	// find the clause with the token argument in the consequent
	Clause tempClause, askClause;
	for (const Clause& clause : kb.getClauses())
	{
		if (id == clause.getConsequent())
		{
			tempClause = clause.getAntecedent();
			// convert each token in the clause to the correct hazard name
			for (Token& token : tempClause.getTokens())
			{
				if (!token.isOperator())
					token = Token(token.getValue().substr(token.getValue().size() - 2, 2));
				askClause << token;
			}
			break;
		}
	}
	// construct a row based on the assumptions
	map<Token, bool> row;
	// tokens not passed in the assumptions are assumed to be false
	for (const Token& token : assumptions)
	{
		if (token.getValue().find(hazard) != string::npos)
			// add it to the row
			row.insert(pair<Token, bool>(Token(token.getValue().substr(token.getValue().size() - 2, 2)), true));
	}
	return clauseResultInRow(askClause, row);
}