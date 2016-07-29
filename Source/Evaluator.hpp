//
//  Evaluator.hpp
//  Inference Engine
//
//  Created by James Ferguson on 4/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef Evaluator_hpp
#define Evaluator_hpp

#include <map>
#include <stack>
#include "Clause.hpp"

using namespace std;


class Evaluator
{
	const map<Token, bool>& context;
	
	class NamedValue
	{
		Token token;
		bool value;
		
	public:
		NamedValue(bool value);
		NamedValue(Token token, bool value);
		NamedValue(Token token, const map<Token, bool>& context);
		
		bool getValue() const;
		bool hasName() const;
		Token getName() const;
	};
	
public:
	Evaluator(const map<Token, bool>& context);
	
	bool operator()(Clause clause) const;
	
private:
	void performOperation(Token op, stack<NamedValue>& valueStack) const;
};

#endif /* Evaluator_hpp */
