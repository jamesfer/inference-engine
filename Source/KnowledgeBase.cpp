//
//  KnowledgeBase.cpp
//  Inference Engine
//
//  Created by James Ferguson on 30/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "KnowledgeBase.hpp"


KnowledgeBase::KnowledgeBase()
{
	
}


KnowledgeBase::KnowledgeBase(vector<Clause> clauses) : clauses(clauses)
{
	for (Clause& clause : clauses)
	{
		for (Token& token : clause.getTokens())
		{
			if (!token.isOperator())
				tokenList.insert(token);
		}
	}
}


vector<Clause> KnowledgeBase::getClauses() const
{
	return clauses;
}


set<Token> KnowledgeBase::getTokens() const
{
	return tokenList;
}


unsigned long KnowledgeBase::size() const
{
	return clauses.size();
}


Clause KnowledgeBase::operator[](unsigned long index) const
{
	return clauses[index];
}


KnowledgeBase& KnowledgeBase::operator<<(Clause clause)
{
	clauses.push_back(clause);
	for (Token token : clause.getTokens())
		// sets only contain unique values so duplicate tokens will be ignored
		if (!token.isOperator())
			tokenList.insert(token);
	return *this;
}


ostream& operator<<(ostream& output, const KnowledgeBase& obj)
{
	if (obj.clauses.size() > 0)
	{
		// Print the first clause
		output << obj.clauses.front();
		
		// Print the rest of the clauses seperated by a comma
		for (unsigned long i = 1; i < obj.clauses.size(); i++)
		{
			output << ", " << obj.clauses[i];
		}
	}
	
	return output;
}