Introduction to AI COS30019
Assignment 2 - Inference Engine 2016

Group ???
James Ferguson 100614728
Matthew Rimkevicus 7678487


*Features
- Text file parser
	Reads a text file containing propositional logic and creates a knowledge base of unique clauses and symbols.

- Forward Chaining
	Searches the knowledge base for a clause it can evaluate based on which symbols it has already evaluated and continues doing so until it evaluates the query.

- Backward Chaining
	Searches the knowledge base for a clause that has the query as the consequent.  It then searches for clauses that have the symbols in the newly found clause as the consequent.  Eventually it finds enough clauses stating facts that it can evaluate the clauses it has found so far, evaluating the query in the process.

- Truth Table
	Uses the knowledge base to create a table of all possible scenarios and counts the number of scenarios where all clauses are true.  It creates a column for each unique symbol and clause, and 2^n rows, n being the number of unique symbols.  The total size of the truth table = (symbols + clauses) * 2^symbols

- Wumpus World program
	Uses the above methods to determine an unknown cell's safety based on the state of known cells.  The search agent prefers safety over blindly jumping into a pit, and will often decide to do nothing as there is not enough information to determine that any cell is safe to move to.

*Test Cases
Test 1
TELL
p2=> p3; p3 => p1; c => e; b&e => f; f&g => h; p1=>d; p1&p3 => c; a; b; p2;
ASK
d

Test 2
TELL
a=>b; b\/c=>d; d&c=>~a; c;
ASK
d

Test Simple
TELL
a; b; a & b => c;
ASK
c

Test Wumpus Board 1
4x4
P: 3 0;
W: 2 1;
G: 3 3;

Test Wumpus Board 2
3x3
P: 2 1;
W: 1 1;
G: 0 2;

Using the incorrect symbols in the input file (eg. using | for OR instead of \/, ! for NOT instead of ~) causes the knowledge base to fail.


*Notes
The truth table would be way too large when used for the Wumpus World program as it would be creating a column for every hazard type in every cell, even a 3x3 grid would create a gigantic truth table.  Glitter and gold do not need to be in the truth table because their relationship is so simple it can be handled by the agent.  Additionally, both the pits and the wumpus follow the same rules for their warning signs: All cells immediately surrounding the pit or wumpus have a warning.  Therefore a lot of information can be trimmed away so that only a single, reasonable sized truth table is necessary to test for pits and the wumpus.

However the truth table is still too inefficient to handle the 5x5 wumpus grid.  The 3x3 grid creates a table of size (3*3 + 3*3) * 2^(3*3) = 9216.  The 5x5 grid creates a table of size (5*5 + 5*5) * 2^(5*5) = 1,677,721,600.  Attempting to run the 5x5 wumpus grid with the truth table will quickly cause the computer to run out of memory.
***DO NOT DO IT***.

*Summary Report
James
- Repurposed their propositional logic parser from the tutorial to create a common knowledge base.
- Created various functions for the knowledge base to do various things.
- Created the forward chaining method.
- Created the backward chaining method.
- Created the Wumpus world board and search agent.

Matthew
- Created the truth table method.
- Retooled the truth table to work much more efficiently with wumpus world.

In this assignment James ended up doing 80% of the work while Matthew did 20%.  Given that James created the 'base' code that the whole program was using, Matthew had to spend a lot of time figuring out exactly what was created and how to use it (more code comments!).