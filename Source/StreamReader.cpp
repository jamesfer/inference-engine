//
//  StreamReader.cpp
//  Inference Engine
//
//  Created by James Ferguson on 30/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "StreamReader.hpp"
#include <fstream>
#include <sstream>
#include "Tokenizer.hpp"
#include "Splitter.hpp"
#include "Converter.hpp"

#include <iostream>


bool StreamReader::read(const char* filename, KnowledgeBase& kb, Clause& clause)
{
	// Open the file
	ifstream input{filename};
	if (!input.is_open())
	{
		cerr << "The input file could not be opened." << endl;
		return false;
	}
	
	
	string blank;
	stringstream tellSection;
	stringstream askSection;
	// Read until the start of the knowledgebase
	while (input >> blank, blank != "TELL");
	
	// Read the contents of the knowledgebase
	while (input >> blank, blank != "ASK")
	{
		tellSection << blank;
	}
	
	// Read the contents of the question
	while (input >> blank)
	{
		askSection << blank;
	}
	
	
	// The input stream split into individual tokens
	vector<Token> tokens = Tokenizer()(tellSection);
	
	// The input split by the ';' character
	vector<vector<Token>> splitTokens = Splitter(";")(tokens);
	
	// Convert each clause to reverse polish
	Converter converter;
	vector<Clause> clauses;
	for (vector<Token>& clause : splitTokens)
	{
		clauses.push_back(converter(clause));
	}
	
	// Build the kb
	kb = KnowledgeBase(clauses);


	// Build ask query
	tokens = Tokenizer()(askSection);
	clause = Clause(tokens);
	
	
	return true;
}