//
//  StreamReader.hpp
//  Inference Engine
//
//  Created by James Ferguson on 30/04/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef StreamReader_hpp
#define StreamReader_hpp

#include "KnowledgeBase.hpp"

using namespace std;


class StreamReader
{
public:
	/**
	 * Reads an input file.
	 * The knowledge base is then stored in kb and the request clause is
	 * stored in clause.
	 */
	static bool read(const char* filename, KnowledgeBase& kb, Clause& clause);
};

#endif /* StreamReader_hpp */
