//
//  ForwardChain.hpp
//  Inference Engine
//
//  Created by James Ferguson on 2/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef ForwardChain_hpp
#define ForwardChain_hpp

#include <ostream>
#include <map>
#include <list>
#include "KnowledgeBase.hpp"
#include "ChainUtility.hpp"

using namespace std;


class ForwardChain : public ChainUtility
{
public:
	ForwardChain(const KnowledgeBase& kb);
	
protected:
	/**
	 * Attempts to deduce the result of the query by repeatedly searching
	 * the knowledge base for known facts.
	 */
	void deduceResult();
	
	/**
	 * Will search for a clause that can be learned from.
	 * Once something is learned will stop searching and return true.
	 * Returns false if no clauses could be learned from.
	 */
	bool parseKB();
	
	
	/**
	 * Returns true if we have found a result yet
	 */
	bool checkForResult();
	
};

#endif /* ForwardChain_hpp */
