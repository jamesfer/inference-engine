//
//  Agent.hpp
//  Inference Engine
//
//  Created by James Ferguson on 20/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef Agent_hpp
#define Agent_hpp

#include <utility>
#include <memory>
#include "Board.hpp"
#include "InferenceEngine.hpp"
#include "Action.hpp"
#include "DijkstraSearcher.hpp"

using namespace std;


class BoardSearchNode
{
	int x, y;
	
	const Board& board;
	InferenceEngine& ie;
	const set<Token>& assumptions;
	
public:
	BoardSearchNode(const Board& board, InferenceEngine& ie, const set<Token>& assumptions, int x, int y);
	
	bool operator<(const BoardSearchNode& other) const;
	bool operator==(const BoardSearchNode& other) const;
	
	vector<pair<shared_ptr<Action>, shared_ptr<BoardSearchNode>>> getActions(const vector<Point>& exploredCells) const;
	
	/**
	 * Returns true if the inference engine implies that this cell has a pit.
	 */
	bool hasPit();
	
	/**
	 * Returns true if the inference engine implies that this cell has the wumpus.
	 */
	bool hasWumpus();
	
	int getX() const;
	int getY() const;
	
private:
	/**
	 * Adds a new action to the action list if it is a valid move.
	 */
	void addAction(vector<pair<shared_ptr<Action>, shared_ptr<BoardSearchNode>>>& actions, const vector<Point>& exploredCells, int x, int y) const;
};






class MoveAction : public Action
{
	// The point were moving towards
	int x, y;
	
public:
	MoveAction(int x, int y, int cost);
	
	virtual string toString() const;
};






class Agent
{
	// The agents current idea of what the outside world looks like
	Board board;
	
	// The list of cells that have been explored by the agent
	vector<Point> exploredCells;
	
	// The current location of the agent in the world
	int x, y;
	
	InferenceEngine& ie;
	
	DijkstraSearcher<BoardSearchNode> searcher;
	
	// The current route that the agent is following
	shared_ptr<Path<BoardSearchNode>> currentPath;
	
	// The step in the route the agent is up to
	int pathStep;
	
public:
	Agent(int width, int height, InferenceEngine& ie);
	
	/**
	 * Tells the agent to learn about the current cell.
	 */
	void percept(const Board& outsideWorld);
	
	/**
	 * Makes the agent decide what action to perform out of.
	 *   - move:x:y
	 *   - grab
	 * The agent itself just returns the action it wants to perform
	 * and doesn't actually do the moving or grabbing.
	 */
	string chooseAction();
	
	/**
	 * Returns the current cell the agent is on
	 */
	Cell& currentCell() const;
	
	int getX() const;
	int getY() const;
	
	void moveTo(int x, int y);
	
private:
	/**
	 * Creates the move action string
	 */
	string move(int x, int y) const;
	
	/**
	 * Returns the list of assumptions that this agent can make about the world.
	 */
	set<Token> getAssumptions() const;
	
	/**
	 * Returns true if the current path exists and we still have steps to perform to complete it.
	 */
	bool currentPathValid() const;
};

#endif /* Agent_hpp */
