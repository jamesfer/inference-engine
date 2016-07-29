//
//  ForwardChain.cpp
//  Inference Engine
//
//  Created by James Ferguson on 2/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "ForwardChain.hpp"
#include "Evaluator.hpp"


ForwardChain::ForwardChain(const KnowledgeBase& kb) : ChainUtility(kb)
{

}


//ostream& operator<<(ostream& output, const ForwardChain& obj)
//{
//	output << obj.result << ": ";
//	
//	map<Token, bool>::const_iterator valueIt = obj.knownValues.begin();
//	output << (*valueIt).first;
//	valueIt++;
//	
//	for (; valueIt != obj.knownValues.end(); valueIt++)
//	{
//		output << ", " << (*valueIt).first;
//	}
//	
//	return output;
//}


//void ForwardChain::prepare()
//{
//	for (Clause& clause : kb.getClauses())
//	{
//		clauses.push_back(clause);
//	}
//}


void ForwardChain::deduceResult()
{
	bool learnedSomething = true;
	bool foundResult = false;
	
	// Continue to loop while the list of clauses is not empty, we can still learn from the list and we have not found the answer yet
	while (!clauses.empty() && learnedSomething && !foundResult)
	{
		learnedSomething = parseKB();
		foundResult = checkForResult();
	}
	
	if (!foundResult)
	{
		// If we've reached here, no definite answer could be found.
		result = false;
	}
}


bool ForwardChain::parseKB()
{
//	Evaluator eval(knownValues);
	
	for (list<Clause>::iterator clauseIt = clauses.begin(); clauseIt != clauses.end(); clauseIt++)
	{
		// Check the special case of a single token
		if (clauseIt->size() == 1)
		{
			// Set the value of the token to true
			knownValues[(*clauseIt)[0]] = true;
			orderAdded.push_back((*clauseIt)[0]);
			clauses.erase(clauseIt);
			return true;
		}
		else
		{
			Clause antecedent = clauseIt->getAntecedent();
			Token consequent = clauseIt->getConsequent();
			
			// Check if we know all the variables in a clause
			set<Token> variables = antecedent.getAllIdentifiers();
			bool haveAllTokens = true;
			for (Token t : variables)
			{
				if (knownValues.find(t) == knownValues.end())
				{
					// We cannot determine the value of this expression
					haveAllTokens = false;
					break;
				}
			}
			
			if (!haveAllTokens)
			{
				continue;
			}
			
			// Evaluate the clause
			knownValues[consequent] = eval(antecedent);
			orderAdded.push_back(consequent);
			
			clauses.erase(clauseIt);
			return true;
		}
	}
	
	// None of the clauses could be learned from
	return false;
}


bool ForwardChain::checkForResult()
{
	// Check if we already know the result
	map<Token, bool>::iterator it = knownValues.find(query);
	if (it != knownValues.end())
	{
		result = ((*it).second);
		return true;
	}
	
	return false;
}


