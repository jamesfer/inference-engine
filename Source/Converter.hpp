//
//  Converter.hpp
//  Postfix Converter
//
//  Created by James Ferguson on 24/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef Converter_hpp
#define Converter_hpp

#include <vector>
#include <map>
#include <string>
#include "Token.hpp"

using namespace std;


class Converter
{
public:
	Converter();
	
	
	vector<Token> operator()(vector<Token> tokenString);
};

#endif /* Converter_hpp */
