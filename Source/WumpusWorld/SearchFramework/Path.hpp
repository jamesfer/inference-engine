#ifndef Path_hpp
#define Path_hpp

#include <vector>
#include <memory>
#include <map>
#include <ostream>
#include "Node.hpp"
#include "Edge.hpp"

using namespace std;


/**
 * Stores a chain of nodes representing a sequence of actions.
 */
template <class State_type>
class Path
{
public:
	typedef Node<State_type> Node_type;
	typedef shared_ptr<Node_type> Node_ptr;
	typedef typename Node_type::Edge_ptr Edge_ptr;
	typedef typename Node_type::State_ptr State_ptr;
	
private:
	/**
	 * Vector of edges that represent all the actions taken in this path.
	 * Starting at the initialState, the edges are applied from the back to the front.
	 */
	vector<Edge_ptr> actions;
	
	
	/**
	 * The initial state
	 */
	State_ptr initialState;
	
	
	/**
	 * List of all nodes that exist in this path
	 */
	vector<Node_ptr> nodes;
	
	
	/**
	 * The cost of this path
	 */
	int pathCost;
	
	
	/**
	 * Number of expansions that it took to find the solution.
	 */
	unsigned long iterations;
	
	
	/**
	 * The number of nodes that were kept in memory in during the search.
	 */
	unsigned long nodeCount;
	
public:
	/**
	 * Constructs a path that starts at deepestChild and follows
	 * the parent chain all the way up to the root node.
	 */
	Path(Node_ptr deepestChild, unsigned long iterations, unsigned long nodeCount) : iterations(iterations), nodeCount(nodeCount)
	{
		// Add the first child element
		nodes.push_back(deepestChild);
		
		Node_ptr parentNode = deepestChild->getParent().lock();
		
		while (parentNode)
		{
			nodes.push_back(parentNode);
			parentNode = parentNode->getParent().lock();
		}
		
		pathCost = deepestChild->getBranchCost();
	}
	
	
	/**
	 * Getter for cost
	 */
	int getCost() const
	{
		return pathCost;
	}
	
	
	/**
	 * Getter for the number of nodes in the path
	 */
	size_t getLength() const
	{
		return nodes.size();
	}
	
	
	/**
	 * Array access operator to return the node at index n
	 */
	Node_ptr& operator[](size_t n)
	{
		return nodes[n];
	}
	
	
	/**
	 * Outputs this path to a stream.
	 * This is the default implementation but this can be overrided by supplying 
	 * template specializations for specific values of State_type
	 */
	friend ostream& operator<<(ostream& output, const Path& obj)
	{
//		output << "Number of actions:             " << obj.nodes.size() - 1 << endl;
//		output << "Number of search iterations:   " << obj.iterations << endl;
//		output << "Number of nodes in memory:     " << obj.nodeCount << endl;
//		output << "Total cost:                    " << obj.getCost() << endl;
		
		// The first node in the path last node in the list and it does not
		// have an edge. So we iterate backwards over the list starting at the
		// second last index.
		for (auto it = ++obj.nodes.rbegin(); it != obj.nodes.rend(); ++it)
		{
			output << *(*it)->getEdge() << ";";
		}
		
		return output;
	}
};

#endif /* Path_hpp */
