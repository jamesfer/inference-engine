//
//  main.cpp
//  Inference Engine
//
//  Created by James Ferguson on 30/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//


#include "StreamReader.hpp"
#include "TruthTable.hpp"
#include "ForwardChain.hpp"
#include "BackwardChain.hpp"
#include <iostream>
#include <fstream>

using namespace std;



/**
 * Prints out any vector of objects that can be printed.
 */
template <class T>
ostream& operator<<(ostream& output, const vector<T>& array)
{
	for (const T& obj : array)
	{
		output << obj << ", ";
	}
	
	return output;
}


int main(int argc, const char* argv[])
{
	if (argc < 3)
	{
		cerr << "Missing input file or method" << endl;
		return 1;
	}
	
	// Read the details from the file
	KnowledgeBase kb;
	Clause clause;
	
	// Read the details from the file
	if (StreamReader::read(argv[1], kb, clause))
	{
		// Print the knowledgebase
		cout << "Knowledge Base:" << endl << kb << endl;

		// Print the ask clause
		cout << "Question:" << endl << clause << endl << endl;

		// Choose method
		const string method = argv[2];
		if (method == "TT")
		{
			TruthTable TT(kb);
			ofstream outFile;
			outFile.open("output.txt");
			if (outFile.good())
				outFile << TT;
			cout << TT << endl;
		}
		else if (method == "FC")
		{
			ForwardChain FC(kb);
			FC.ask(clause[0]);
			cout << FC << endl;
		}
		else if (method == "BC")
		{
			BackwardChain BC(kb);
			BC.ask(clause[0]);
			cout << BC << endl;
		}
		else
		{
			cerr << "Unknown method" << endl;
			return 2;
		}

		return 0;
	}
	else
	{
		cerr << "Input file could not be read" << endl;
		return 2;
	}
}
