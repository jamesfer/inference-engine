//
//  Converter.cpp
//  Postfix Converter
//
//  Created by James Ferguson on 24/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "Converter.hpp"
#include <stack>


Converter::Converter()
{

}


vector<Token> Converter::operator()(vector<Token> tokenString)
{
	vector<Token> outputStream;
	stack<Token> operatorStack;
	
	for (Token token : tokenString)
	{
		// If it is an open bracket add it without checking precedence
		if (token == "(")
		{
			operatorStack.push(token);
		}
		// If it is a close bracket, pop all symbols off until the open
		// bracket is found.
		else if (token == ")")
		{
			while (operatorStack.top() != "(")
			{
				outputStream.push_back(operatorStack.top());
				operatorStack.pop();
			}
			
			// Pop the open bracket without outputting it.
			operatorStack.pop();
		}
		// Token is a plain identifier, add it straight to the output.
		else if (!token.isOperator())
		{
			outputStream.push_back(token);
		}
		// Token is an operator.
		else
		{
			// Pop all higher precedence operators off the stack.
			while (operatorStack.size() > 0
				   && Token::operators.at(token) < Token::operators.at(operatorStack.top()))
			{
				outputStream.push_back(operatorStack.top());
				operatorStack.pop();
			}
			
			// Then add the token to the stack
			operatorStack.push(token);
		}
	}
	
	// Pop all remaining operators off the stack
	while (!operatorStack.empty())
	{
		outputStream.push_back(operatorStack.top());
		operatorStack.pop();
	}
	
	return outputStream;
}

