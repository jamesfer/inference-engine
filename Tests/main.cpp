//
//  main.cpp
//  Tests
//
//  Created by James Ferguson on 4/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Evaluator.hpp"
#include "KnowledgeBaseUtils.hpp"


void test()
{
//	map<Token, bool> context;
//	context[Token("a")] = true;
//	context[Token("b")] = true;
//	context[Token("c")] = false;
//	
//	
//	Evaluator e(context);
//	
//	vector<pair<Clause, TripleBool>> tests;
//	tests.push_back({
//		Clause({Token("a"), Token("b"), Token("&")}), YES
//	});
//	tests.push_back({
//		Clause({Token("a"), Token("b"), Token("!"), Token("&")}), NO
//	});
//	tests.push_back({
//		Clause({Token("a"), Token("c"), Token("|")}), YES
//	});
//	tests.push_back({
//		Clause({Token("a"), Token("c"), Token("&")}), NO
//	});
//	tests.push_back({
//		Clause({Token("a"), Token("b"), Token("c"), Token("|"), Token("&")}), YES
//	});
//	tests.push_back({
//		Clause({Token("a"), Token("b"), Token("=>")}), YES
//	});
//	tests.push_back({
//		Clause({Token("a"), Token("c"), Token("=>")}), NO
//	});
//	tests.push_back({
//		Clause({Token("a"), Token("c"), Token("!"), Token("=>")}), YES
//	});
//	
//	
//	for (vector<pair<Clause, TripleBool>>::iterator it = tests.begin(); it != tests.end(); it++)
//	{
//		cout << (*it).second << ": " << e((*it).first) << endl;
//	}
}


void testKBUtils()
{
//	cout << KnowledgeBaseUtils::makeToken("glitter", 1, 3); << endl;
	
	cout << KnowledgeBaseUtils::generateKB(4, 4) << endl;
}


int main(int argc, const char * argv[]) {
//	test();
	testKBUtils();
}









