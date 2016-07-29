//
//  Agent.cpp
//  Inference Engine
//
//  Created by James Ferguson on 20/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "Agent.hpp"
#include <vector>
#include "KnowledgeBaseUtils.hpp"



BoardSearchNode::BoardSearchNode(const Board& board, InferenceEngine& ie, const set<Token>& assumptions, int x, int y) : board(board), ie(ie), assumptions(assumptions), x(x), y(y)
{
	
}


bool BoardSearchNode::operator<(const BoardSearchNode& other) const
{
	if (x != other.x)
	{
		return x < other.x;
	}
	else
	{
		return y < other.y;
	}
}


bool BoardSearchNode::operator==(const BoardSearchNode& other) const
{
	return x == other.x && y == other.y;
}


vector<pair<shared_ptr<Action>, shared_ptr<BoardSearchNode>>> BoardSearchNode::getActions(const vector<Point>& exploredCells) const
{
	vector<pair<shared_ptr<Action>, shared_ptr<BoardSearchNode>>> actions;
	
	// Add each direction
	addAction(actions, exploredCells, x + 1, y);
	addAction(actions, exploredCells, x - 1, y);
	addAction(actions, exploredCells, x, y + 1);
	addAction(actions, exploredCells, x, y - 1);
	
	return actions;
}


bool BoardSearchNode::hasPit()
{
	return ie.ask(KnowledgeBaseUtils::makeToken("pit", x, y), assumptions);
}


bool BoardSearchNode::hasWumpus()
{
	return ie.ask(KnowledgeBaseUtils::makeToken("wumpus", x, y), assumptions);
}


int BoardSearchNode::getX() const
{
	return x;
}


int BoardSearchNode::getY() const
{
	return y;
}


void BoardSearchNode::addAction(vector<pair<shared_ptr<Action>, shared_ptr<BoardSearchNode> > > &actions, const vector<Point>& exploredCells, int x, int y) const
{
	if (board.inBounds(x, y))
	{
		shared_ptr<BoardSearchNode> newNode = make_shared<BoardSearchNode>(board, ie, assumptions, x, y);
		
		// Cost is 1000 if the cell is unsafe
		int cost = ((find(exploredCells.begin(), exploredCells.end(), Point{ x, y }) == exploredCells.end())
			&& (newNode->hasPit() || newNode->hasWumpus())) ? 1000 : 1;
		
		shared_ptr<Action> action = static_pointer_cast<Action>(make_shared<MoveAction>(x, y, cost));
		
		actions.push_back({
			action,
			newNode
		});
	}
}







MoveAction::MoveAction(int x, int y, int cost) : x(x), y(y)
{
	this->cost = cost;
}


string MoveAction::toString() const
{
	return "move:" + to_string(x) + ":" + to_string(y);
}







Agent::Agent(int width, int height, InferenceEngine& ie) : ie(ie), x(0), y(0), board(Board(width, height))
{
	
}


void Agent::percept(const Board& outsideWorld)
{
	currentCell().glitter = outsideWorld[x][y].glitter;
	currentCell().breeze = outsideWorld[x][y].breeze;
	currentCell().stench = outsideWorld[x][y].stench;
	exploredCells.push_back({x, y});
}


string Agent::chooseAction()
{
	// Check if the current cell is glittering
	if (currentCell().glitter)
	{
		return "grab";
	}
	
	// Check if we need to find a new target
	else if (!currentPathValid())
	{
		// The set of known facts
		set<Token> assumptions = getAssumptions();
		
		// Create the search node at the current location
		BoardSearchNode curNode(board, ie, assumptions, x, y);

		// The shortest known path to the next safe node
		shared_ptr<Path<BoardSearchNode>> shortestPath = nullptr;
		
		// Loop through all the cells
		for (int x = 0; x < board.width(); x++)
		{
			for (int y = 0; y < board.height(); y++)
			{
				// Skip this cell if it has already been explored
				if (find(exploredCells.begin(), exploredCells.end(), Point{x, y}) != exploredCells.end())
				{
					continue;
				}
				
				// Skip this cell if it is not safe
				if (ie.ask(KnowledgeBaseUtils::makeToken("pit", x, y), assumptions)
					|| ie.ask(KnowledgeBaseUtils::makeToken("wumpus", x, y), assumptions))
				{
					continue;
				}
				
		
				// The target location
				BoardSearchNode targetNode(board, ie, assumptions, x, y);
				
				// Find the path to this node
				searcher.reset();
				shared_ptr<Path<BoardSearchNode>> path = searcher.search(curNode, targetNode, exploredCells);
				
				// Check if this path is better
				if (!shortestPath || path->getCost() < shortestPath->getCost())
				{
					shortestPath = path;
				}
			}
		}
		
		
		// Now save the new path
		if (shortestPath)
		{
			currentPath = shortestPath;
			pathStep = (int) currentPath->getLength() - 2;
		}
	}
	
	
	// Check if we have a path to follow
	if (currentPathValid())
	{
		shared_ptr<BoardSearchNode> nextState = (*currentPath)[pathStep]->getState();
		pathStep--;
		return move(nextState->getX(), nextState->getY());
	}
		
		
		
		// Pick a direction to move
//		if (board.inBounds(x, y + 1))
//		{
//			return move(x, y + 1);
//		}
//		else if (board.inBounds(x + 1, y))
//		{
//			return move(x + 1, y);
//		}
//		else if (board.inBounds(x, y - 1))
//		{
//			return move(x, y - 1);
//		}
//		else
//		{
//			return move(x - 1, y);
//		}
		
	// No cells were safe, just return nothing
	return "";
}


Cell& Agent::currentCell() const
{
	return board[x][y];
}


int Agent::getX() const
{
	return x;
}


int Agent::getY() const
{
	return y;
}


void Agent::moveTo(int x, int y)
{
	this->x = x;
	this->y = y;
}


string Agent::move(int x, int y) const
{
	return "move:" + to_string(x) + ":" + to_string(y);
}


set<Token> Agent::getAssumptions() const
{
	set<Token> assumptions;
	
	for (int x = 0; x < board.width(); x++)
	{
		for (int y = 0; y < board.height(); y++)
		{
			// Check if we have explored this cell
			if (find(exploredCells.begin(), exploredCells.end(), Point{x, y}) != exploredCells.end())
			{
				// Add the things we know about the cell into assumptions
				Cell& current = board[x][y];
				
				if (current.breeze)
				{
					assumptions.insert(KnowledgeBaseUtils::makeToken("breeze", x, y));
				}
				
				if (current.stench)
				{
					assumptions.insert(KnowledgeBaseUtils::makeToken("stench", x, y));
				}
				
				if (current.glitter)
				{
					assumptions.insert(KnowledgeBaseUtils::makeToken("glitter", x, y));
				}
			}
			
			// If we have not explored this cell, assume that it is dangerous.
			// This means that it has a breeze and a stench and no gold.
			else
			{
				assumptions.insert(KnowledgeBaseUtils::makeToken("breeze", x, y));
				assumptions.insert(KnowledgeBaseUtils::makeToken("stench", x, y));
			}
		}
	}
	
	
	return assumptions;
}


bool Agent::currentPathValid() const
{
	return currentPath && pathStep >= 0;
}



