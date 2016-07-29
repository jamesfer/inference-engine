#ifndef Action_hpp
#define Action_hpp

#include <string>

using namespace std;


/**
 * An action causes a transition from one state to another.
 * The main purpose of this class is to allow the display of a path by printing
 * each action as a string.
 */
class Action
{
protected:
	int cost = 1;
	
public:
	/**
	 * Getter for cost
	 */
	int getCost() const
	{
		return cost;
	}
	
	
	/**
	 * Should return the string representation of the action that will be 
	 * displayed when a path is found.
	 */
	virtual string toString() const = 0;
};

#endif /* Action_hpp */
