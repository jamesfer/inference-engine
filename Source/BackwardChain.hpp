//
//  BackwardChain.hpp
//  Inference Engine
//
//  Created by James Ferguson on 7/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef BackwardChain_hpp
#define BackwardChain_hpp

#include "ChainUtility.hpp"
#include <list>
#include <ostream>
#include <stack>
#include "KnowledgeBase.hpp"

using namespace std;


class BackwardChain : public ChainUtility
{	
	struct ExpressionNode
	{
		Token token;
		Clause clause;
		
		ExpressionNode(const Token& token);
		
		bool hasClause() const;
		
	};
	
	
	stack<ExpressionNode> agenda;
	
public:
	BackwardChain(const KnowledgeBase& kb);
	
protected:
	virtual void deduceResult() override;
};

#endif /* BackwardChain_hpp */
