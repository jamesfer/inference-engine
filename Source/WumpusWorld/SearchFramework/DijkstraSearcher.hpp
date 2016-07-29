#ifndef DijkstraSearcher_hpp
#define DijkstraSearcher_hpp

#include "Searcher.hpp"
#include <set>


/**
 * Comparator that sorts nodes via their branch cost
 */
template <class State_type>
struct DijkstraComparator
{
	typedef shared_ptr<Node<State_type>> Node_ptr;
	
	bool operator()(const Node_ptr& a, const Node_ptr& b) const
	{
		return a->getBranchCost() < b->getBranchCost();
	}
};


/**
 * Dijkstra's algorithm orders nodes via their total path cost and therefore the 
 * next node in the frontier is always the one with the lowest path cost.
 */
template <class State_type, class Success_functor = IsGoalCheck<State_type>>
class DijkstraSearcher : public Searcher<State_type, Success_functor>
{
public:
	typedef Searcher<State_type, Success_functor> Super;
	typedef typename Super::Node_ptr Node_ptr;
	typedef typename Super::Path_ptr Path_ptr;
	
protected:
	multiset<Node_ptr, DijkstraComparator<State_type>> frontier;
	
public:
	DijkstraSearcher() : Super(false)
	{
		
	}
	
	
	DijkstraSearcher(Success_functor functor) : Super(functor, false)
	{
		
	}
	
	
	/**
	 * Also clears the frontier
	 */
	virtual void reset() override
	{
		Super::reset();
		frontier.clear();
	}
	
	
	/**
	 * Insert nodes into their correct sorted position
	 */
	virtual void addToFrontier(Node_ptr node) override
	{
		frontier.insert(node);
	}
	
	
	/**
	 * Returns the size of the frontier
	 */
	virtual unsigned long frontierSize() const override
	{
		return frontier.size();
	}
	
	
	/**
	 * Pops nodes of of the front of the frontier
	 */
	virtual Node_ptr nextInFrontier() override
	{
		Node_ptr next = *frontier.begin();
		frontier.erase(frontier.begin());
		return next;
	}
	
	
	/**
	 * Like the BFS search, the entire tree can be checked for duplicate nodes.
	 * However, we cannnot check the frontier as some paths in the frontier may 
	 * not be optimal, and therefore this node may be a better option.
	 */
	virtual bool isRepeatedState(Node_ptr newChild) const override
	{
		// Search the tree
		if (this->tree.find(newChild) != this->tree.end())
		{
			return true;
		}
		
		return false;
	}
};

#endif /* DijkstraSearcher_hpp */
