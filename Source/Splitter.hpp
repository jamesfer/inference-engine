//
//  Splitter.hpp
//  Postfix Converter
//
//  Created by James Ferguson on 29/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef Splitter_hpp
#define Splitter_hpp

#include <vector>
#include <string>
#include "Token.hpp"

using namespace std;


struct Splitter
{
private:
	string separator;
	
public:
	Splitter(string separator);
	
	
	vector<vector<Token>> operator()(vector<Token> stream);
};

#endif /* Splitter_hpp */
