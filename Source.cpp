#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
using namespace std;

struct SudokuPuzzle
{
	int maxRows;
	int maxCols;

	int** puzzle;
	bool** locked;

	void PrintDottedLine(int maxColumns)
	{
		cout << "+";
		for (size_t i = 0; i < maxColumns * 2 + 3; i++)
		{
			cout << "-";
		}
		cout << "+" << endl;
	}
	void Print()
	{
		PrintDottedLine(maxCols);
		for (size_t r = 0; r < maxRows; r++)
		{
			for (size_t c = 0; c < maxCols; c++)
			{
				cout << "|" << puzzle[r][c];
				if (c == 2 || c == 5)
				{
					cout << "| ";
				}
			}
			cout << "|" << endl;
			if (r == 2 || r == 5) 
			{
				cout << endl;
			}
			
		}
		PrintDottedLine(maxCols);
		cout << endl;
	}
};

struct Coords
{
	int row;
	int col;
	int num;
};

template <typename T>
class Stack
{
private:
	T* mData;
	int mTop = -1;
public:
	Stack(int missingNumbers) { mData = new T[missingNumbers]; }
	~Stack() { delete[] mData; }
	bool isEmpty() { return mTop == -1; }
	void push(T el)
	{
		mTop++;
		mData[mTop] = el;
	}

	void pop()
	{
		mTop--;
	}

	T top()
	{
		return mData[mTop];
	}
};

SudokuPuzzle* LoadPuzzle(string filename)
{
	ifstream input;
	input.open(filename);
	if (!input.is_open())
	{
		cout << "error opening " << filename << endl;
	}
	SudokuPuzzle* pSP = new SudokuPuzzle;
	input >> pSP->maxRows;
	input >> pSP->maxCols;

	string line;
	getline(input, line);

	pSP->puzzle = new int*[pSP->maxRows];
	pSP->locked = new bool*[pSP->maxRows];
	for (size_t r = 0; r < pSP->maxRows; r++)
	{
		pSP->puzzle[r] = new int[pSP->maxCols];
		pSP->locked[r] = new bool[pSP->maxCols];

		getline(input, line);
		stringstream lineStream(line);

		int c = 0;
		int num;
		while (lineStream >> num)
		{
			if (num != ' ')
			{
				pSP->puzzle[r][c] = num;
				if (num == 0) pSP->locked[r][c] = false;
				else pSP->locked[r][c] = true;
				c++;
			}
		}
	}

	input.close();
	return pSP;
}

bool CanBePlacedInRow(SudokuPuzzle* pS, int n, int row)
{
	for (size_t c = 0; c < pS->maxRows; c++)
	{
		if (pS->puzzle[row][c] == n) return false;
	}
	return true;
}
bool CanBePlacedInColumn(SudokuPuzzle* pS, int n, int col)
{
	for (size_t r = 0; r < pS->maxCols; r++)
	{
		if (pS->puzzle[r][col] == n) return false;
	}
	return true;
}
bool CanBePlacedInBox(SudokuPuzzle* pS, int n, int row, int col)
{
	int minRow, maxRow, minCol, maxCol;
	switch (row / 3)
	{
	case 0: minRow = 0; maxRow = 2; break;
	case 1: minRow = 3; maxRow = 5; break;
	case 2: minRow = 6; maxRow = 8; break;
	default: break;
	}
	switch (col / 3)
	{
	case 0: minCol = 0; maxCol = 2; break;
	case 1: minCol = 3; maxCol = 5; break;
	case 2: minCol = 6; maxCol = 8; break;
	default: break;
	}

	for (size_t r = minRow; r <= maxRow; r++)
	{
		for (size_t c = minCol; c <= maxCol; c++)
		{
			if (pS->puzzle[r][c] == n) return false;
		}
	}
	return true;
}

void Solve(SudokuPuzzle* pSP)
{
	Stack<Coords> s(pSP->maxRows * pSP->maxCols);
	for (int r = 0; r < pSP->maxRows; r++)
	{
		for (int c = 0; c < pSP->maxCols; c++)
		{
			if (pSP->locked[r][c]) continue;
			int n = pSP->puzzle[r][c];

			for (int n = 1; n <= 9; n++)
			{
				if (CanBePlacedInColumn(pSP, n, c) && CanBePlacedInRow(pSP, n, r) && CanBePlacedInBox(pSP, n, r, c))
				{
					s.push({ r, c, n });
					pSP->puzzle[r][c] = n;
					break;
				}
				else
				{
					//backtrack
					if (n == 9)
					{
						if (s.isEmpty())
						{
							cout << "No solution" << endl;
							return;
						}

						Coords coord = s.top();
						s.pop();

						while (!s.isEmpty() && coord.num == 9)
						{
							pSP->puzzle[coord.row][coord.col] = 0;
							coord = s.top();
							s.pop();
						}

						r = coord.row;
						c = coord.col;
						n = coord.num;
						pSP->puzzle[coord.row][coord.col] = 0;
					}
				}
			}
		}
	}
}

void main()
{
	SudokuPuzzle* pSP = LoadPuzzle("puzzle1.txt");
	pSP->Print();
	
	Solve(pSP);
	
	pSP->Print();
	
	//deletes
	for (size_t i = 0; i < pSP->maxRows; i++)
	{
		delete[] pSP->puzzle[i];
	}
	delete[] pSP->puzzle;
	delete pSP;
}