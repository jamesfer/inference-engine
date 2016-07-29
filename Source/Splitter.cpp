//
//  Splitter.cpp
//  Postfix Converter
//
//  Created by James Ferguson on 29/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "Splitter.hpp"


Splitter::Splitter(string separator) : separator(separator)
{
	
}


vector<vector<Token>> Splitter::operator()(vector<Token> stream)
{
	vector<vector<Token>> result = {{}};
	
	for (Token &token : stream)
	{
		if (token == separator)
		{
			result.push_back({});
		}
		else
		{
			result.back().push_back(token);
		}
	}
	
	// Remove the last list if empty
	if (result.back().size() == 0)
	{
		result.pop_back();
	}
	
	return result;
}