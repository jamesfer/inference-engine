//
//  KnowledgeBaseUtils.cpp
//  Inference Engine
//
//  Created by James Ferguson on 15/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "KnowledgeBaseUtils.hpp"



KnowledgeBase KnowledgeBaseUtils::generateKB(size_t width, size_t height)
{
	KnowledgeBase kb;
	
	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			// Add in the hazards
			addGlitterClause(kb, x, y);
			addStenchClause(kb, x, y, width, height);
			addBreezeClause(kb, x, y, width, height);
		}
	}
	
	return kb;
}


Token KnowledgeBaseUtils::makeToken(string hazard, size_t x, size_t y)
{
	return Token(hazard + to_string(x) + to_string(y));
}


void KnowledgeBaseUtils::addGlitterClause(KnowledgeBase &kb, size_t x, size_t y)
{
	Clause clause;
	clause << makeToken("glitter", x, y);
	clause << makeToken("gold", x, y);
	clause << Token(IMPL);
	kb << clause;
}


void KnowledgeBaseUtils::addStenchClause(KnowledgeBase &kb, size_t x, size_t y, size_t width, size_t height)
{
	Clause clause;
	vector<Token> hazardTokens;
	
	// List all the cells that must have the hazard in them to imply the location of the wumpus
	if (x > 0)
	{
		hazardTokens.push_back(makeToken("stench", x - 1, y));
	}
	if (x < width - 1)
	{
		hazardTokens.push_back(makeToken("stench", x + 1, y));
	}
	if (y > 0)
	{
		hazardTokens.push_back(makeToken("stench", x, y - 1));
	}
	if (y < height - 1)
	{
		hazardTokens.push_back(makeToken("stench", x, y + 1));
	}
	
	if (hazardTokens.size() > 0)
	{
		clause << hazardTokens[0];
		
		// Add the rest of the tokens with the AND token in reverse polish order
		for (size_t i = 1; i < hazardTokens.size(); i++)
		{
			clause << hazardTokens[i] << Token(AND);
		}
		
		// Add the implication part
		clause << makeToken("wumpus", x, y);
		clause << Token(IMPL);
		kb << clause;
	}
}


void KnowledgeBaseUtils::addBreezeClause(KnowledgeBase &kb, size_t x, size_t y, size_t width, size_t height)
{
	Clause clause;
	vector<Token> hazardTokens;
	
	// List all the cells that must have the hazard in them to imply the location of the wumpus
	if (x > 0)
	{
		hazardTokens.push_back(makeToken("breeze", x - 1, y));
	}
	if (x < width - 1)
	{
		hazardTokens.push_back(makeToken("breeze", x + 1, y));
	}
	if (y > 0)
	{
		hazardTokens.push_back(makeToken("breeze", x, y - 1));
	}
	if (y < height - 1)
	{
		hazardTokens.push_back(makeToken("breeze", x, y + 1));
	}
	
	if (hazardTokens.size() > 0)
	{
		clause << hazardTokens[0];
		
		// Add the rest of the tokens with the AND token in reverse polish order
		for (size_t i = 1; i < hazardTokens.size(); i++)
		{
			clause << hazardTokens[i] << Token(AND);
		}
		
		// Add the implication part
		clause << makeToken("pit", x, y);
		clause << Token(IMPL);
		kb << clause;
	}
}








