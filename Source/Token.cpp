//
//  Token.cpp
//  Inference Engine
//
//  Created by James Ferguson on 30/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "Token.hpp"
#include <utility>


map<Token, unsigned short> Token::operators = {
	{{NOT}, 5},
	{{AND}, 4},
	{{OR}, 3},
	{{IMPL}, 2},
	{{BIDIR}, 1},
	{{"("}, 0},
	{{")"}, 0},
	{{";"}, 0}
};


Token::Token()
{

}


Token::Token(string value) : value(value)
{

}


string Token::getValue() const
{
	return value;
}


bool Token::isOperator() const
{
	for (operator_type op : operators)
	{
		if (op.first == value)
		{
			return true;
		}
	}
	
	return false;
}


bool Token::operator==(const Token& other) const
{
	return value == other.value;
}


bool Token::operator!=(const Token& other) const
{
	return !operator==(other);
}


bool Token::operator==(const string& other) const
{
	return value == other;
}


bool Token::operator!=(const string& other) const
{
	return !operator==(other);
}


bool Token::operator<(const Token &other) const
{
	return value < other.value;
}


ostream& operator<<(ostream& output, const Token& obj)
{
	output << obj.value;
	return output;
}
