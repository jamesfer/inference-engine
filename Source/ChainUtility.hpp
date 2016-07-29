//
//  ChainUtility.hpp
//  Inference Engine
//
//  Created by James Ferguson on 7/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef ChainUtility_hpp
#define ChainUtility_hpp

#include <list>
#include <map>
#include <set>
#include "KnowledgeBase.hpp"
#include "Evaluator.hpp"
#include "InferenceEngine.hpp"

using namespace std;


class ChainUtility : public InferenceEngine
{
protected:
	const KnowledgeBase& kb;
	Token query;
	
	list<Clause> clauses;
	map<Token, bool> knownValues;
	list<Token> orderAdded;
	bool result;
	
	const Evaluator eval;
	
	
	ChainUtility(const KnowledgeBase& kb);
	
	
	/**
	 * Should perform reasoning on the kb to determine the value of the token.
	 */
	virtual void deduceResult() = 0;
	
	
	/**
	 * Returns true if all the identifiers in the set are known.
	 */
	bool areValuesKnown(const set<Token> ids) const;
	
	
	/**
	 * Returns true if the token's value is known
	 */
	bool isValueKnown(const Token& id) const;
	
	
	friend ostream& operator<<(ostream& output, const ChainUtility& obj);
	
	
private:
	/**
	 * Extracts all clauses from the kb and stores them in a manipulatable list.
	 * Any single token clauses are added to the known values map.
	 */
	void prepare();
	
	
public:
	virtual ~ChainUtility();
	virtual bool ask(const Token& id, const set<Token>& assumptions = {});
};

#endif /* ChainUtility_hpp */
