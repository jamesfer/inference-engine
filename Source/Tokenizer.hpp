//
//  Tokenizer.hpp
//  Postfix Converter
//
//  Created by James Ferguson on 24/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <istream>
#include <vector>
#include "BufferedInputStream.hpp"
#include "Token.hpp"

using namespace std;


class Tokenizer
{
//	vector<string> operators;
	
	
public:
	Tokenizer();
	
	
	vector<Token> operator()(istream& input);
	
private:
	/**
	 * Returns the number of operators that start with a given string
	 */
	int operatorsStartingWith(string partial);
	
	
	/**
	 * Returns true if an operator exactly matches a given string.
	 */
	bool isOperator(string token);
	
	
	/**
	 * Checks if the next sequence in the input is an operator.
	 * If it is, stores the result in op.
	 * Note: does not remove the characters from the input string.
	 */
	bool isOperatorNext(BufferedInputStream& input, string& op);
	
	
	
	/**
	 * Reads in the next token from a stream.
	 */
	Token readToken(BufferedInputStream& input);
};

#endif /* Tokenizer_hpp */
