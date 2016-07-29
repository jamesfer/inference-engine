//
//  Clause.hpp
//  Inference Engine
//
//  Created by James Ferguson on 30/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef Clause_hpp
#define Clause_hpp

#include <vector>
#include <string>
#include <ostream>
#include <set>
#include "Token.hpp"

using namespace std;


class Clause
{
	vector<Token> tokens;
	
public:
	Clause();
	Clause(vector<Token> tokens);
	
	/**
	 * Returns the antecedent of a clause (part before the implication).
	 * Assumes the clause is in horn form.
	 *
	 */
	Clause getAntecedent() const;
	
	
	/**
	 * Returns the consequent of a clause (part after the implication).
	 * Assumes the clause is in horn form.
	 */
	Token getConsequent() const;
	
	
	/**
	 * Returns a set of all non-operation tokens.
	 */
	set<Token> getAllIdentifiers() const;
	
	
	vector<Token> getTokens() const;
	unsigned long size() const;
	
	bool operator==(const Clause& other) const;
	bool operator!=(const Clause& other) const;
	Token operator[](unsigned long index) const;
	bool operator<(const Clause& other) const;
	
	Clause& operator<<(Token token);
	
	friend ostream& operator<<(ostream& output, const Clause& obj);
};

#endif /* Clause_hpp */
