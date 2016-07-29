//
//  KnowledgeBaseUtils.hpp
//  Inference Engine
//
//  Created by James Ferguson on 15/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef KnowledgeBaseUtils_hpp
#define KnowledgeBaseUtils_hpp

#include "KnowledgeBase.hpp"

using namespace std;


class KnowledgeBaseUtils
{
public:
	static KnowledgeBase generateKB(size_t width, size_t height);
	
	static Token makeToken(string hazard, size_t x, size_t y);
	
private:
	static void addGlitterClause(KnowledgeBase& kb, size_t x, size_t y);
	static void addStenchClause(KnowledgeBase& kb, size_t x, size_t y, size_t width, size_t height);
	static void addBreezeClause(KnowledgeBase& kb, size_t x, size_t y, size_t width, size_t height);
};

#endif /* KnowledgeBaseUtils_hpp */
