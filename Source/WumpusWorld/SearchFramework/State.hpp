#ifndef State_hpp
#define State_hpp

#include <vector>
#include <utility>
#include <type_traits>
#include "Action.hpp"

using namespace std;


/**
 * Test to check if state type is valid
 */
template <class T>
class ValidState
{
	// Template that enforces the correct signature
	template <class U, vector<pair<shared_ptr<Action>, shared_ptr<U>>> (U::*)(const vector<Point>& exploredCells) const>
	struct getActions {};
	
	// Template that enforces the correct signature
	template <class U, bool (U::*)(const U&) const>
	struct boolOperator {};
	
	// When U has a member add that can matches the template type of SFINAE,
	// this template speciailization is chosen. A pointer to the struct is
	// required so we can pass 0, instead of having to initialize an empty struct
	// for no reason.
	template <class U>
	static true_type getActionsTest(getActions<U, &U::getActions>*);
	
	// When U does not have a member called add, the template system falls back
	// to choosing this option
	template <class U>
	static false_type getActionsTest(...);
	
	template <class U>
	static true_type lessThanTest(boolOperator<U, &U::operator<>*);
	
	template <class U>
	static false_type lessThanTest(...);
	
	template <class U>
	static true_type equivalenceTest(boolOperator<U, &U::operator==>*);
	
	template <class U>
	static false_type equivalenceTest(...);
	
public:
	// These structs are used to determine if certain methods are present
	struct hasGetActions : decltype(getActionsTest<T>(0)) {};
	struct hasLessThanOp : decltype(lessThanTest<T>(0)) {};
	struct hasEquivOp : decltype(equivalenceTest<T>(0)) {};
};


/**
 * Macro that checks all the conditions required for a state type to be valid.
 */
#define VALIDATE_STATE_TYPE(T) \
	static_assert(is_copy_constructible<T>::value, "Given state type must be copy constructable"); \
	static_assert(ValidState<T>::hasGetActions::value, \
		"Given type requires a valid getActions function"); \
	static_assert(ValidState<T>::hasLessThanOp::value, \
		"Given type requires a less than operator"); \
	static_assert(ValidState<T>::hasEquivOp::value, \
		"Given type requires an equivalence operator");

#endif /* State_hpp */

