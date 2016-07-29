#ifndef Searcher_hpp
#define Searcher_hpp

#include <deque>
#include <memory>
#include <exception>
#include <iostream>
#include <set>
#include "Edge.hpp"
#include "Node.hpp"
#include "Path.hpp"

#define MAXIMUM_SEARCH_ITERATIONS 100000
#define STR(s) #s
#define XSTR(s) STR(s)

using namespace std;



/**
 * Exception that is thrown when there are no more nodes in the frontier.
 */
class NoSolutionExistsException : public exception
{
	virtual const char* what() const throw()
	{
		return "There was way to reach the goal from the start point that you specified.";
	}
};


/**
 * Exception that is thrown when the search algorithm hits 
 * the MAXIMUM_SEARCH_ITERATIONS limit.
 */
class CouldNotFindSolutionException : public exception
{
	virtual const char* what() const throw()
	{
		return "The searcher reached its maximum iteration limit of " XSTR(MAXIMUM_SEARCH_ITERATIONS)" and still could not find a solution.";
	}
};


/**
 * The default functor used in searchers.
 * Checks whether the current state is equivalent to the goal state.
 */
template <class State_type>
struct IsGoalCheck
{
private:
	const State_type* goal;
	
public:
	IsGoalCheck()
	{
		
	}
	
	void setGoal(const State_type* goal)
	{
		this->goal = goal;
	}
	
	bool operator()(const State_type& state) const
	{
		return state == *goal;
	}
};


/**
 * The searcher is responsible for finding a path from one state to another.
 * It is an abstract class that does not specify the behaviour of the frontier.
 */
template <class State_type, class Success_functor = IsGoalCheck<State_type>>
class Searcher
{
	// Validate the state template
	VALIDATE_STATE_TYPE(State_type);
	
public:
	typedef Node<State_type> Node_type;
	typedef shared_ptr<Node_type> Node_ptr;
	typedef typename Node_type::State_ptr State_ptr;
	typedef typename Node_type::Edge_ptr Edge_ptr;
	typedef shared_ptr<Path<State_type>> Path_ptr;
	
private:
	/**
	 * Comparator used to order nodes in the tree by their state.
	 */
	struct NodeComparator
	{
		bool operator()(Node_ptr a, Node_ptr b) const
		{
			return (*a->getState()) < (*b->getState());
		}
	};
	
public:
	typedef multiset<Node_ptr, NodeComparator> Tree_type;
	
	
protected:
	/**
	 * When the searcher is in precheck mode, each node is checked if it is a 
	 * goal before being put into the frontier which can save a lot of time when
	 * using BFS style searchers. If precheckSuccess is false, nodes are checked 
	 * when they are removed from the front of the frontier. This is the most 
	 * ideal style for heuristic style searchers as it means when the goal is 
	 * found it is guarenteed to be optimal because it was the first node in the 
	 * search.
	 */
	const bool precheckSuccess = true;
	
	
	/**
	 * Vector that stores a reference of all nodes in the tree to ensure 
	 * that they are not garbage collected.
	 */
	Tree_type tree;
	
	
	/**
	 * Functor used for checking if the current state is the desired one.
	 */
	Success_functor successCheck;
	
	
	/**
	 * The number of iterations that the searcher has performed
	 */
	unsigned long searchIterations;
	
	
	/**
	 * Returns the next node in the frontier that should be expanded.
	 */
	virtual Node_ptr nextInFrontier() = 0;
	
	
	/**
	 * Add a new node to the frontier.
	 */
	virtual void addToFrontier(Node_ptr node) = 0;
	
	
	/**
	 * Returns the number of elements in the frontier.
	 */
	virtual unsigned long frontierSize() const = 0;
	
	
	/**
	 * Add multiple nodes to the frontier at once.
	 */
	virtual void addToFrontier(vector<Node_ptr> nodes)
	{
		for (Node_ptr node : nodes)
		{
			addToFrontier(node);
		}
	}
	
	
	/**
	 * Determines if the node can be skipped because the state has already been 
	 * checked. By default, runs up all the parents of the given node and checks
	 * if the state has existed before.
	 */
	virtual bool isRepeatedState(Node_ptr newChild) const
	{
		shared_ptr<Node_type> node = newChild->getParent().lock();
		State_ptr state = newChild->getState();
		
		while (node)
		{
			if (node->isState(*state))
			{
				return true;
			}
			
			// Since the states don't match, we now check the parent.
			node = node->getParent().lock();
		}
		
		return false;
	}
	
	
	/**
	 * Helper function to create new edges
	 */
	virtual Edge_ptr makeEdge(Node_ptr node, shared_ptr<Action> action) const
	{
		return make_shared<Edge<State_type>>(node, action);
	}
	
	
	/**
	 * Helper function to create new nodes
	 */
	virtual Node_ptr makeNode(State_ptr state, Edge_ptr edge = nullptr) const
	{
		return make_shared<Node_type>(state, edge);
	}
	
	
	/**
	 * Helper function to build a path from the given node all the way to the 
	 * root
	 */
	Path_ptr buildPath(Node_ptr newNode) const
	{
		// We found our desired state
		return make_shared<Path<State_type>>(newNode, searchIterations, getNodeCount());
	}
	
	
	/**
	 * Checks whether the current node matches a goal state. Is only enabled 
	 * if the Success_functor accepts a State_ptr type.
	 */
	template <bool (Success_functor::*)(const State_type&) const = &Success_functor::operator()>
	bool checkForSuccess(Node_ptr curNode) const
	{
		return successCheck(*curNode->getState());
	}
	
	
	/**
	 * Checks whether the current node matches a goal state. Is only enabled
	 * if the Success_functor accepts a Node_ptr type.
	 */
	template <bool (Success_functor::*)(const Node<State_type>&) const = &Success_functor::operator()>
	bool checkForSuccess(Node_ptr curNode) const
	{
		return successCheck(*curNode);
	}
	
	
	/**
	 * Helper function to check if multiple nodes a goal at once. If any of them 
	 * are goals the fucntion returns true and the goal is stored in goalNode
	 */
	bool checkForSuccess(vector<Node_ptr> curNodes, Node_ptr& goalNode) const
	{
		for (Node_ptr& curNode : curNodes)
		{
			if (checkForSuccess(curNode))
			{
				goalNode = curNode;
				return true;
			}
		}
		
		return false;
	}
	
	
	/**
	 * Adds a node to the tree
	 */
	void addToTree(Node_ptr newNode)
	{
		*tree.insert(newNode);
	}
	
	
	/**
	 * Creates a new node for each neighbour of the current node.
	 * Will perform repeated state checks and only return valid options.
	 */
	vector<Node_ptr> expand(Node_ptr node, const vector<Point>& exploredCells) const
	{
		vector<Node_ptr> childNodes;
		
		// Edges is a vector of pairs <state, action>
		vector<pair<shared_ptr<Action>, State_ptr>> actionStatePairs = node->getState()->getActions(exploredCells);
		
		for (auto& actionStatePair : actionStatePairs)
		{
			// Create a new edge
			Edge_ptr edge = makeEdge(node, actionStatePair.first);
			
			// Create a new node
			Node_ptr newNode = makeNode(actionStatePair.second, edge);
			
			// Skip any state that has already been visited in this chain
			if (!isRepeatedState(newNode))
			{
				childNodes.push_back(newNode);
			}
		}
		
		return childNodes;
	}
	
	
	/**
	 * The main implementation of the search algorithm.
	 * When successful, the found path will be returned. If unsuccesful will 
	 * throw either a NoSolutionsExistsException if the frontier is empty or a
	 * CouldNotFindSolutionException if the search reaches the maximum number of
	 * search iterations.
	 */
	Path_ptr startIteration(const State_type& initial, const vector<Point>& exploredCells)
	{
		// We need to copy the initial state otherwise garbarge collection will
		// try to deallocate it after the search completes
		State_ptr initialCopy = State_ptr{new State_type(initial)};
		Node_ptr curNode = makeNode(initialCopy);
		
		// Used to store a reference to the goal node if found
		Node_ptr goalNode = nullptr;
		
		// Stores the neighbours of a node when expanded
		vector<Node_ptr> childNodes;
		
		
		// Precheck mode checks nodes before they go into the frontier
		if (precheckSuccess)
		{
			if (checkForSuccess(curNode))
			{
				return buildPath(curNode);
			}
		}
		
		// Add the initial node to the frontier
		addToFrontier(curNode);
		
		do {
			// Pop the next node off the frontier
			curNode = nextInFrontier();
			addToTree(curNode);
			
			// Non-precheck mode checks nodes after they come out of the frontier
			if (!precheckSuccess)
			{
				if (checkForSuccess(curNode))
				{
					return buildPath(curNode);
				}
			}
			
			// Find the children of the current node
			childNodes = expand(curNode, exploredCells);
			
			// Check each child node for success before putting in the frontier
			if (precheckSuccess)
			{
				if (checkForSuccess(childNodes, goalNode))
				{
					return buildPath(goalNode);
				}
			}
			
			// Since none of the children are goals, add them all to the frontier
			addToFrontier(childNodes);
			
			// Check if we have reached the maximum iterations allowed
			++searchIterations;
			if (searchIterations >= MAXIMUM_SEARCH_ITERATIONS)
			{
				throw CouldNotFindSolutionException();
			}
		} while (frontierSize() != 0);
		
		// If the loop breaks without finding a solution, it means the frontier
		// was emptied
		throw NoSolutionExistsException();
	}
	
	
	/**
	 * Helper function to ensure that the searcher is reset even if an exception 
	 * is thrown.
	 */
	Path_ptr attemptSearch(const State_type& initial)
	{
		try
		{
			Path_ptr path = startIteration(initial);
			reset();
			return path;
		}
		catch (exception e)
		{
			reset();
			throw e;
		}
	}
	
	
public:
	/**
	 * Creates a searcher by default initializing the success functor.
	 */
	Searcher(bool precheckSuccess = true) : successCheck(), precheckSuccess(precheckSuccess), searchIterations(0)
	{
		
	}
	
	
	/**
	 * Creates a searcher with a specific success functor instance.
	 */
	Searcher(Success_functor successCheck, bool precheckSuccess = true) : successCheck(successCheck), precheckSuccess(precheckSuccess), searchIterations(0)
	{
	
	}
	
	
	virtual ~Searcher()
	{
	
	}
	
	
	/**
	 * Responsible to clearing all the memory used by the searcher during the 
	 * search algorithm.
	 */
	virtual void reset()
	{
		tree.clear();
		searchIterations = 0;
	}
	
	
	/**
	 * The public method that performs the search from an initial state.
	 * Only enabled when the success functor is not the goal check kind.
	 */
	template <class T = Success_functor, class = typename enable_if<!is_same<T, IsGoalCheck<State_type>>::value>::type>
	Path_ptr search(const State_type& initial, const vector<Point>& exploredCells)
	{
		return startIteration(initial, exploredCells);
	}

	
	/**
	 * The public method that performs the search from an initial state and 
	 * searching for a specific goal.
	 * Only enabled when the success functor is the goal check kind.
	 */
	template <class T = Success_functor, class = typename enable_if<is_same<T, IsGoalCheck<State_type>>::value>::type>
	Path_ptr search(const State_type& initial, const State_type& goal, const vector<Point>& exploredCells)
	{
		// Set the goal state of the goal check functor
		successCheck.setGoal(&goal);
		
		// Perform the search
		Path_ptr path = startIteration(initial, exploredCells);
		
		// Clear the goal from the functor
		successCheck.setGoal(nullptr);
		
		return path;
	}
	
	
	/**
	 * Getter for search iterations
	 */
	unsigned long getSearchIterations() const
	{
		return searchIterations;
	}
	
	
	/**
	 * Getter for node count
	 */
	unsigned long getNodeCount() const
	{
		return tree.size() + frontierSize();
	}
};

#endif /* Searcher_hpp */











