//
//  InferenceEngine.hpp
//  Inference Engine
//
//  Created by James Ferguson on 20/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef InferenceEngine_hpp
#define InferenceEngine_hpp

#include <set>
#include "Token.hpp"

using namespace std;


class InferenceEngine
{
public:
	virtual ~InferenceEngine() {};
	virtual bool ask(const Token& id, const set<Token>& knownValues = {}) = 0;
};

#endif /* InferenceEngine_hpp */
