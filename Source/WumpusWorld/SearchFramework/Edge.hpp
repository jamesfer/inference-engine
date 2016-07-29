#ifndef Edge_hpp
#define Edge_hpp

#include <ostream>
#include <memory>
#include <string>
#include "Action.hpp"
#include "State.hpp"

using namespace std;

template <class State_type>
class Node;


/**
 * An edge is the link between nodes in the search tree. Each edge is stored by 
 * a node and then the edge keeps the reference to the parent node.
 */
template <class State_type>
class Edge
{
public:
	typedef Node<State_type> Node_type;
	typedef weak_ptr<Node_type> Node_ptr;
	
private:
	/**
	 * Weak reference to the parent.
	 */
	Node_ptr parent;
	
	
	/**
	 * The action that was taken to get from the parent node to the current one
	 */
	shared_ptr<Action> action;
	
public:
	/**
	 * Creates an edge
	 */
	Edge(Node_ptr parent, shared_ptr<Action> action) : parent(parent), action(action)
	{
		
	}
	
	
	virtual ~Edge()
	{
		
	}
	
	
	/**
	 * Getter for parent
	 */
	Node_ptr getParent() const
	{
		return parent;
	}
	
	
	/**
	 * Getter for action
	 */
	shared_ptr<Action> getAction() const
	{
		return action;
	}
	
	
	/**
	 * Stream output operator that prints the string representation of the 
	 * action.
	 */
	friend ostream& operator<<(ostream& output, const Edge& obj)
	{
		output << obj.action->toString();
		return output;
	}
};

#endif /* Edge_hpp */
