//
//  Tokenizer.cpp
//  Postfix Converter
//
//  Created by James Ferguson on 24/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "Tokenizer.hpp"


Tokenizer::Tokenizer()
{
	
}


vector<Token> Tokenizer::operator()(istream &input)
{
	BufferedInputStream bInput{input};
	vector<Token> tokenStream;
	
	bInput.skipWhitespace();
	while (!bInput.eof())
	{
		tokenStream.push_back(readToken(bInput));
		bInput.skipWhitespace();
	}
	
	return tokenStream;
}


int Tokenizer::operatorsStartingWith(string partial)
{
	int count = 0;
	
	for (Token::operator_type op : Token::operators)
	{
		if (op.first.getValue().compare(0, partial.size(), partial) == 0)
		{
			count++;
		}
	}
	
	return count;
}


bool Tokenizer::isOperator(string token)
{
	return Token(token).isOperator();
}


bool Tokenizer::isOperatorNext(BufferedInputStream& input, string& op)
{
	string matchingOperator;
	
	// Read one character from the input
	string token{input.getChar()};
	
	// Keep reading in chars until no operators match
	while (operatorsStartingWith(token) > 0)
	{
		// If there is an exact match
		if (isOperator(token))
		{
			// Store the successful match
			matchingOperator = token;
			
			// If there possibly is a longer matching operator, keep looking
			// Otherwise we can break now
			if (operatorsStartingWith(matchingOperator) == 1)
			{
				break;
			}
		}
		
		// Read the next character
		token += input.getChar();
	}
	
	// Put all characters back into the input
	while (!token.empty())
	{
		input << token.back();
		token.pop_back();
	}
	
	// Check if a match was found
	if (matchingOperator != "")
	{
		op = matchingOperator;
		return true;
	}
	else
	{
		return false;
	}
}


Token Tokenizer::readToken(BufferedInputStream& input)
{
	string token = "";
	string nextOp = "";
	char next = input.peekChar();
	
	// Continue to read chars as long as we have not reached
	// a space or operator
	while (next != EOF && !isspace(next) && !isOperatorNext(input, nextOp))
	{
		token += input.getChar();
		next = input.peekChar();
	}
	
	// No contents was read which means the next string is an operator
	if (token == "")
	{
		// No operator was read
		if (nextOp == "")
		{
			throw runtime_error("No contents was read from the stream. Likely because the first character was whitespace.");
		}
		else
		{
			token = nextOp;
			// Since nextOp was never extracted from the stream, we need
			// to skip some characters
			input.skip(token.size());
		}
	}
	
	return Token(token);
}








