//
//  BufferedInputStream.hpp
//  Postfix Converter
//
//  Created by James Ferguson on 24/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef BufferedInputStream_hpp
#define BufferedInputStream_hpp

#include <istream>
#include <stack>

using namespace std;


class BufferedInputStream
{
	stack<char> buffer;
	istream& input;
	
public:
	BufferedInputStream(istream& input);
	
	
	/**
	 * Pops a character of the buffer. If the buffer is empty, reads one from
	 * the input stream.
	 */
	BufferedInputStream& operator>>(char& c);
	
	
	/**
	 * Adds a character to the buffer to be read next.
	 */
	BufferedInputStream& operator<<(const char& c);
	
	
	/**
	 * Returns the next character in the stream.
	 */
	char getChar();
	
	
	/**
	 * Returns the next character but does not extract it from the stream.
	 */
	char peekChar();
	
	
	/**
	 * Skips the next n characters in the stream or buffer.
	 */
	void skip(unsigned long n);
	
	
	/**
	 * Returns true if the stream is at the end.
	 */
	bool eof() const;
	
	
	/**
	 * Skips all whitespace chars from the input
	 */
	void skipWhitespace();
};

#endif /* BufferedInputStream_hpp */
