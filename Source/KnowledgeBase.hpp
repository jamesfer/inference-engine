//
//  KnowledgeBase.hpp
//  Inference Engine
//
//  Created by James Ferguson on 30/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef KnowledgeBase_hpp
#define KnowledgeBase_hpp


#include <vector>
#include <set>
#include <ostream>
#include "Clause.hpp"

using namespace std;


class KnowledgeBase
{
	vector<Clause> clauses;
	set<Token> tokenList;
	
public:
	KnowledgeBase();
	KnowledgeBase(vector<Clause> clauses);
	
	vector<Clause> getClauses() const;
	set<Token> getTokens() const;
	unsigned long size() const;
	
	Clause operator[](unsigned long index) const;
	
	KnowledgeBase& operator<<(Clause clause);
	
	friend ostream& operator<<(ostream& output, const KnowledgeBase& obj);
};


//typedef KnowledgeBase_template<"!", "&", "|", "=>", "<=>", "(", ")", ";"> KnowledgeBase;

#endif /* KnowledgeBase_hpp */
