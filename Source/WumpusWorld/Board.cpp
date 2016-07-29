//
//  Board.cpp
//  Inference Engine
//
//  Created by James Ferguson on 14/05/2016.
//  Copyright © 2016 James Ferguson. All rights reserved.
//

#include "Board.hpp"
#include <iomanip>



bool Point::operator==(const Point &other) const
{
	return x == other.x && y == other.y;
}





Board::Board(int width, int height)
{
	resize(width, height);
}


vector<Cell>& Board::operator[](size_t x) const
{
	return (vector<Cell>&)grid[x];
}


Cell& Board::operator[](Point p) const
{
	return (*this)[p.x][p.y];
}


istream& operator>>(istream& input, Board& obj)
{
	// Read in the dimensions of the board
	size_t width, height;
	input >> width;
	input.get();
	input >> height;
	obj.resize(width, height);
	
	// Reset every cell to empty
	for (vector<Cell>& col : obj.grid)
	{
		for (Cell& cell : col)
		{
			cell.breeze = false;
			cell.stench = false;
			cell.glitter = false;
			cell.pit = false;
			cell.wumpus = false;
			cell.gold = false;
		}
	}
	
	// Read in hazards
	char hazardType;
	input >> hazardType;
	while (input.good())
	{
		if (hazardType != 'P' && hazardType != 'W' && hazardType != 'G')
		{
			// Continue to read in characters
			continue;
		}
		
		char blank;
		
		// Skip the colon
		input >> blank;
		
		// Read points while we don't encounter another character
		input >> ws;
		blank = input.peek();
		while (input.good() && blank != 'P' && blank != 'W' && blank != 'G')
		{
			// Read x and y
			size_t x, y;
			input >> x;
			input >> y;
			
			// Add the hazard
			if (hazardType == 'P')
			{
				obj.addPit(x, y);
			}
			else if (hazardType == 'W')
			{
				obj.addWumpus(x, y);
			}
			else if (hazardType == 'G')
			{
				obj.addGold(x, y);
			}
			
			// Skip the separator
			input >> blank;
			
			// Test the next character
			input >> ws;
			blank = input.peek();
		}
		
		input >> hazardType;
	}
	
	return input;
}


ostream& operator<<(ostream& output, Board& obj)
{
	if (obj.grid.size() > 0)
	{
		// Print out the col numbers
		cout << "   ";
		for (size_t x = 0; x < obj.grid.size(); x++)
		{
			cout << setw(4) << setfill(' ') << x << "  ";
		}
		cout << endl;
		
		for (size_t y = 0; y < obj.grid[0].size(); y++)
		{
			// Print the top of the cell
			cout << "   ";
			for (size_t x = 0; x < obj.grid.size(); x++)
			{
				cout << "+-----";
			}
			cout << "+" << endl;
			
			// Print out the details
			cout << setw(2) << setfill(' ') << y << " ";
			for (size_t x = 0; x < obj.grid.size(); x++)
			{
				cout << "| ";
				cout << (obj.grid[x][y].breeze ? "B" : " ");
				cout << (obj.grid[x][y].stench ? "S" : " ");
				cout << (obj.grid[x][y].glitter ? "l" : " ");
				cout << " ";
			}
			cout << "|" << endl;
			cout << "   ";
			for (size_t x = 0; x < obj.grid.size(); x++)
			{
				cout << "| ";
				cout << (obj.grid[x][y].pit ? "P" : " ");
				cout << (obj.grid[x][y].wumpus ? "W" : " ");
				cout << (obj.grid[x][y].gold ? "G" : " ");
				cout << " ";
			}
			cout << "|" << endl;
		}
	}
	
	// Print the bottom line
	cout << "   ";
	for (size_t x = 0; x < obj.grid.size(); x++)
	{
		cout << "+-----";
	}
	cout << "+" << endl;
	
	return output;
}


int Board::width() const
{
	return (int) grid.size();
}


int Board::height() const
{
	return (int) (grid.size() > 0 ? grid[0].size() : 0);
}


bool Board::inBounds(int x, int y) const
{
	return x >= 0 && x < width() && y >= 0 && y < height();
}


void Board::resize(const size_t width, const size_t height)
{
	grid.resize(width);
	for (vector<Cell>& col : grid)
	{
		col.resize(height);
	}
}


void Board::addPit(const size_t x, const size_t y)
{
	grid[x][y].pit = true;
	
	if (x > 0)
	{
		grid[x - 1][y].breeze = true;
	}
	if (x < grid.size() - 1)
	{
		grid[x + 1][y].breeze = true;
	}
	if (y > 0)
	{
		grid[x][y - 1].breeze = true;
	}
	if (y < grid[x].size() - 1)
	{
		grid[x][y + 1].breeze = true;
	}
}


void Board::addWumpus(const size_t x, const size_t y)
{
	grid[x][y].wumpus = true;
	
	if (x > 0)
	{
		grid[x - 1][y].stench = true;
	}
	if (x < grid.size() - 1)
	{
		grid[x + 1][y].stench = true;
	}
	if (y > 0)
	{
		grid[x][y - 1].stench = true;
	}
	if (y < grid[x].size() - 1)
	{
		grid[x][y + 1].stench = true;
	}
}


void Board::addGold(const size_t x, const size_t y)
{
	grid[x][y].gold = true;
	grid[x][y].glitter = true;
}








