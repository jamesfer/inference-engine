//
//  BufferedInputStream.cpp
//  Postfix Converter
//
//  Created by James Ferguson on 24/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "BufferedInputStream.hpp"


BufferedInputStream::BufferedInputStream(istream& input) : input(input)
{
	
}


BufferedInputStream& BufferedInputStream::operator>>(char& c)
{
	c = getChar();
	return *this;
}


BufferedInputStream& BufferedInputStream::operator<<(const char& c)
{
	buffer.push(c);
	return *this;
}


char BufferedInputStream::getChar()
{
	char c;
	
	if (!buffer.empty())
	{
		c = buffer.top();
		buffer.pop();
	}
	else
	{
		input >> c;
	}
	
	return c;
}


char BufferedInputStream::peekChar()
{
	if (!buffer.empty())
	{
		return buffer.top();
	}
	else
	{
		return (char) input.peek();
	}
}


void BufferedInputStream::skip(unsigned long n)
{
	char c;
	for (unsigned int i = 0; i < n; i++)
	{
		*this >> c;
	}
}


bool BufferedInputStream::eof() const
{
	return buffer.empty() && input.eof();
}


void BufferedInputStream::skipWhitespace()
{
	bool reachedNonWhitespace = false;
	
	while (!reachedNonWhitespace)
	{
		if (!buffer.empty())
		{
			if (isspace(buffer.top()))
			{
				buffer.pop();
			}
			else
			{
				reachedNonWhitespace = true;
			}
		}
		else
		{
			input >> ws;
			reachedNonWhitespace = true;
		}
	}
}


