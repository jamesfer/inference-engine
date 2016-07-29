//
//  Clause.cpp
//  Inference Engine
//
//  Created by James Ferguson on 30/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "Clause.hpp"


Clause::Clause()
{
	
}


Clause::Clause(vector<Token> tokens) : tokens(tokens)
{
	
}


Clause Clause::getAntecedent() const
{
	vector<Token> antecedentTokens;
	
	if (size() >= 3)
	{
		for (size_t i = 0; i < size() - 2; i++)
		{
			antecedentTokens.push_back(tokens[i]);
		}
	}
	
	return Clause(antecedentTokens);
}


Token Clause::getConsequent() const
{
	return tokens[size() - 2];
}


set<Token> Clause::getAllIdentifiers() const
{
	set<Token> ids;
	
	for (Token t : getTokens())
	{
		if (!t.isOperator())
		{
			ids.insert(t);
		}
	}
	
	return ids;
}


vector<Token> Clause::getTokens() const
{
	return tokens;
}


unsigned long Clause::size() const
{
	return tokens.size();
}


bool Clause::operator==(const Clause& other) const
{
	if (tokens.size() != other.tokens.size())
	{
		return false;
	}
	
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] != other.tokens[i])
		{
			return false;
		}
	}
	
	return true;
}


bool Clause::operator!=(const Clause& other) const
{
	return !operator==(other);
}


Token Clause::operator[](unsigned long index) const
{
	return tokens[index];
}

bool Clause::operator<(const Clause& other) const
{
	// return something arbitrary so long as *this < other and other !< *this
	for (unsigned int i = 0; (i < size()) && (i < other.size()); i++)
	{
		if (tokens[i] != other.tokens[i])
			return tokens[i] < other.tokens[i];
	}
	// all tokens up to here are the same
	return size() < other.size();
}


Clause& Clause::operator<<(Token token)
{
	tokens.push_back(token);
	return *this;
}


ostream& operator<<(ostream& output, const Clause& obj)
{
	for (unsigned long i = 0; i < obj.tokens.size(); i++)
	{
		// Print the token
		output << obj.tokens[i];
		
		
		// Print a space if the token is not the last
		if (i != obj.tokens.size() - 1 && obj.tokens[i] != "!")
		{
			output << " ";
		}
	}
	
	return output;
}


