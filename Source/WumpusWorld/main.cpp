//
//  main.cpp
//  WumpusWorld
//
//  Created by James Ferguson on 14/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Board.hpp"
#include "Agent.hpp"
#include "ForwardChain.hpp"
#include "BackwardChain.hpp"
#include "TruthTable.hpp"
#include "KnowledgeBaseUtils.hpp"

using namespace std;


int main(int argc, const char * argv[]) {
	if (argc < 3)
	{
		cerr << "Not enough arguments present." << endl;
		return 1;
	}
	
	ifstream input(argv[1]);
	if (!input.is_open())
	{
		cerr << "Could not open world file." << endl;
		cerr << argv[1] << endl;
		return 2;
	}
	
	
	
	
	// Read in the board
	Board board(0, 0);
	input >> board;
	
	// Create the knowledge base
	KnowledgeBase kb = KnowledgeBaseUtils::generateKB(board.width(), board.height());
	
	// Create the inference engine
	InferenceEngine* ie;
	string method{argv[2]};
	if (method == "FC")
	{
		ie = new ForwardChain(kb);
	}
	else if (method == "BC")
	{
		ie = new BackwardChain(kb);
	}
	else if (method == "TT")
	{
		cout << "Warning: The truth table can take a very, very, very long time to compile. It is recommended that you use the FC or BC methods instead" << endl;
		ie = new TruthTable(kb, true);
	}
	else
	{
		throw runtime_error("Inference method is not valid.");
	}
	
	// Create the agent
	Agent agent{board.width(), board.height(), *ie};
	
	
	
	
	cout << "The current board:" << endl;
	cout << board << endl << endl ;
	cout << "The agent starts at point 0,0" << endl;
	cout << "Actions:" << endl;
	
	
	int actionCount = 0;
	string result;
	while (true)
	{
		Cell& realCell = board[agent.getX()][agent.getY()];
		
		// Check if the agent is dead
		if (realCell.pit)
		{
			result = "pit";
			break;
		}
		if (realCell.wumpus)
		{
			result = "wumpus";
			break;
		}
		
		// Let the agent learn about the world
		agent.percept(board);
		
		// Perform an action
		string action = agent.chooseAction();
		actionCount++;
		
		cout << action << ", ";
		
		// Check if the agent has won
		if (action == "grab")
		{
			if (realCell.gold) {
				result = "gold";
				break;
			}
		}
		
		// Check if the agent couldn't think of a move
		else if (action == "")
		{
			result = "none";
			break;
		}
		
		// Move the agent to a new cell if it wants to
		else if (action.compare(0, 4, "move:"))
		{
			size_t index = action.find_last_of(':');
			string xStr = action.substr(5, index - 5);
			string yStr = action.substr(index + 1);
			
			agent.moveTo(atoi(xStr.c_str()), atoi(yStr.c_str()));
		}
	}
	cout << endl << endl;
	
	
	
	// Print the result
	if (result == "pit" || result == "wumpus")
	{
		cout << "You lost!" << endl;
		
		if (result == "pit")
		{
			cout << "You fell into a pit" << endl;
		}
		else
		{
			cout << "You were eaten by the wumpus" << endl;
		}
	}
	else if (result == "gold")
	{
		cout << "You won!" << endl;
		cout << "You found the gold" << endl;
	}
	else if (result == "none")
	{
		cout << "You could not find a safe path through the cave" << endl;
	}
	
	cout << "Actions: " << actionCount << endl;
	
	delete ie;
	return 0;
}
