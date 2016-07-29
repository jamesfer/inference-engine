//
//  Evaluator.cpp
//  Inference Engine
//
//  Created by James Ferguson on 4/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "Evaluator.hpp"



Evaluator::NamedValue::NamedValue(bool value) : token(""), value(value)
{
	
}


Evaluator::NamedValue::NamedValue(Token token, bool value) : token(token), value(value)
{
	
}


Evaluator::NamedValue::NamedValue(Token token, const map<Token, bool>& context) : token(token)
{
	if (context.find(token) == context.end())
	{
		value = false;
	}
	else
	{
		value = context.at(token);
	}
}


bool Evaluator::NamedValue::getValue() const
{
	return value;
}


bool Evaluator::NamedValue::hasName() const
{
	return token.getValue().size() != 0;
}


Token Evaluator::NamedValue::getName() const
{
	return token;
}






Evaluator::Evaluator(const map<Token, bool>& context) : context(context)
{
	
}


bool Evaluator::operator()(Clause clause) const
{
	stack<NamedValue> valueStack;
	
	for (size_t i = 0; i < clause.getTokens().size(); i++)
	{
		Token token = clause.getTokens()[i];
		
		if (token.isOperator())
		{
			performOperation(token, valueStack);
		}
		// If the token is a identifier
		else
		{
			valueStack.push(NamedValue(token, context));
		}
	}
	
	return valueStack.top().getValue();
}


void Evaluator::performOperation(Token op, stack<NamedValue>& valueStack) const
{
	// Unary operators
	NamedValue right = valueStack.top();
	valueStack.pop();
	
	if (op == NOT)
	{
		if (right.getValue())
		{
			valueStack.push(false);
		}
		else
		{
			valueStack.push(true);
		}
		return;
	}
	
	// Binary operators
	NamedValue left = valueStack.top();
	valueStack.pop();
	
	if (op == AND)
	{
		if (right.getValue() && left.getValue())
		{
			valueStack.push(true);
		}
		else
		{
			valueStack.push(false);
		}
	}
	else if (op == OR)
	{
		if (right.getValue() || left.getValue())
		{
			valueStack.push(true);
		}
		else
		{
			valueStack.push(false);
		}
	}
	else if (op == IMPL)
	{
		// Results in true when left is false, or both are true
		if (!left.getValue() || (left.getValue() && right.getValue()))
		{
			valueStack.push(true);
		}
		else
		{
			valueStack.push(false);
		}
	}
	else if (op == BIDIR)
	{
		// Results in true when both sides the same
		if (right.getValue() == left.getValue())
		{
			valueStack.push(true);
		}
		else
		{
			valueStack.push(false);
		}
	}
}












