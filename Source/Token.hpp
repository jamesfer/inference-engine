//
//  Token.hpp
//  Inference Engine
//
//  Created by James Ferguson on 30/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef Token_hpp
#define Token_hpp

#include <vector>
#include <map>
#include <string>
#include <ostream>

// Operators
#define AND		"&"
#define OR		"\\/"
#define NOT		"~"
#define BIDIR	"<=>"
#define IMPL	"=>"

using namespace std;


class Token
{
	string value;
	
public:
	static map<Token, unsigned short> operators;
	typedef pair<Token, unsigned short> operator_type;

	Token();
	Token(string value);

	string getValue() const;
	bool isOperator() const;

	bool operator==(const Token& other) const;
	bool operator!=(const Token& other) const;
	bool operator==(const string& other) const;
	bool operator!=(const string& other) const;
	bool operator<(const Token& other) const;

	friend ostream& operator<<(ostream& output, const Token& obj);
};

#endif /* Token_hpp */
