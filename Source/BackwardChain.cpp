//
//  BackwardChain.cpp
//  Inference Engine
//
//  Created by James Ferguson on 7/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "BackwardChain.hpp"



BackwardChain::ExpressionNode::ExpressionNode(const Token& token) : token(token)
{
	
}


bool BackwardChain::ExpressionNode::hasClause() const
{
	return clause.size() != 0;
}




BackwardChain::BackwardChain(const KnowledgeBase& kb) : ChainUtility(kb)
{
	
}


void BackwardChain::deduceResult()
{
	agenda.push(ExpressionNode(query));
	
	while (!agenda.empty())
	{
		ExpressionNode& current = agenda.top();
		
		// Check if the token is already known
		if (isValueKnown(current.token))
		{
			current.clause = Clause({current.token});
		}
		
		// If the expression does not have a clause we will attempt to find one for it.
		if (!current.hasClause())
		{
			// We know have to find a expression that will prove the token
			list<Clause>::iterator clauseIt = clauses.begin();
			for (; clauseIt != clauses.end(); clauseIt++)
			{
				// If the clause is a plain fact
				if (clauseIt->size() == 1)
				{
					if ((*clauseIt)[0] == current.token)
					{
						knownValues[current.token] = true;
						orderAdded.push_back(current.token);
						clauses.erase(clauseIt);
						break;
					}
				}
				// If the clause is an expression
				else if (clauseIt->getConsequent() == current.token)
				{
					// We have found an expression that proves the token
					current.clause = clauseIt->getAntecedent();
					clauses.erase(clauseIt);
					break;
				}
			}
			
			// Check if we have found the value of the token or a clause to imply it
			if (!isValueKnown(current.token) && !current.hasClause())
			{
				// So we can just assume that it is false
				knownValues[current.token] = false;
			}
		}
		
		// If a clause has been found for this expression, attempt to resolve it
		if (current.hasClause())
		{
			// Check if we can solve the clause of the current expression
			if (areValuesKnown(current.clause.getAllIdentifiers()))
			{
				knownValues[current.token] = eval(current.clause);
				orderAdded.push_back(current.token);
			}
			else
			{
				// Add all the tokens in the clause to the agenda
				set<Token> newTokens = current.clause.getAllIdentifiers();
				for (Token t : newTokens)
				{
					agenda.push(ExpressionNode(t));
				}
				
				// Continue the loop so that the expression is not popped off of the stack.
				continue;
			}
		}
	
		// Remove the current node from the stack.
		agenda.pop();
	}
	
	if (isValueKnown(query))
	{
		result = knownValues[query];
	}
	else
	{
		result = false;
	}
}


