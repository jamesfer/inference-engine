//
//  ChainUtility.cpp
//  Inference Engine
//
//  Created by James Ferguson on 7/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "ChainUtility.hpp"
#include <iostream>


ChainUtility::ChainUtility(const KnowledgeBase& kb) : kb(kb), result(false), knownValues(), eval({knownValues})
{
	
}


bool ChainUtility::areValuesKnown(const set<Token> ids) const
{
	for (const Token& id : ids)
	{
		if (knownValues.find(id) == knownValues.end())
		{
			return false;
		}
	}
	
	return true;
}


bool ChainUtility::isValueKnown(const Token &id) const
{
	return knownValues.find(id) != knownValues.end();
}


ostream& operator<<(ostream& output, const ChainUtility& obj)
{
	output << (obj.result ? "YES" : "NO") << ": ";
	
	list<Token>::const_iterator valueIt = obj.orderAdded.begin();
	if (valueIt != obj.orderAdded.end())
	{
		output << *valueIt;
		valueIt++;
	}
	for (; valueIt != obj.orderAdded.end(); valueIt++)
	{
		output << ", " << *valueIt;
	}
	
	return output;
}


void ChainUtility::prepare()
{
	clauses.clear();
	knownValues = {};
	result = false;
	
	for (Clause& clause : kb.getClauses())
	{
		clauses.push_back(clause);
	}
}


ChainUtility::~ChainUtility()
{
	
}


bool ChainUtility::ask(const Token& id, const set<Token>& assumptions)
{
	// Reset the data from the last query
	prepare();
	
	// Add the query and assumptions
	query = id;
	for (const Token& tok : assumptions)
	{
		knownValues[tok] = true;
	}
	
	// Find the results
	deduceResult();
	
	return result;
}






