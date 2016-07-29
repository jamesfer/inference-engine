//
//  Board.hpp
//  Inference Engine
//
//  Created by James Ferguson on 14/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <vector>
#include <iostream>

using namespace std;


struct Point
{
	int x, y;
	
	bool operator==(const Point& other) const;
};


struct Cell
{
	bool breeze;
	bool stench;
	bool glitter;
	
	bool pit;
	bool wumpus;
	bool gold;
};


class Board
{
	vector<vector<Cell>> grid;
	
	
public:
	Board(int width, int height);
	
	vector<Cell>& operator[](size_t x) const;
	Cell& operator[](Point p) const;
	
	friend istream& operator>>(istream& input, Board& obj);
	
	friend ostream& operator<<(ostream& output, Board& obj);
	
	int width() const;
	int height() const;
	
	bool inBounds(int x, int y) const;
	
private:
	void resize(const size_t width, const size_t height);
	
	void addPit(const size_t x, const size_t y);
	void addWumpus(const size_t x, const size_t y);
	void addGold(const size_t x, const size_t y);
};

#endif /* Board_hpp */
