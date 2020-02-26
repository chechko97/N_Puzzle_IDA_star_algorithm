#include "Tile.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <string>

using namespace std;

int finalZeroPosition;
unsigned int rows;

vector<Tile> generateAdjTiles(Tile current, Tile prev) {
	vector<Tile> adjTiles;
	int x = current.getX();
	int y = current.getY();

	if (x > 0) {
		Tile* temp = new Tile(x - 1, y);
		if (*temp != prev)
			adjTiles.push_back(*temp);
	}

	if (y > 0) {
		Tile* temp = new Tile(x, y - 1);
		if (*temp != prev)
			adjTiles.push_back(*temp);
	}

	if (x < rows - 1) {
		Tile* temp = new Tile(x + 1, y);
		if (*temp != prev)
			adjTiles.push_back(*temp);
	}

	if (y < rows - 1) {
		Tile* temp = new Tile(x, y + 1);
		if (*temp != prev)
			adjTiles.push_back(*temp);
	}

	return adjTiles;
}

unsigned int getManhattonDistance(int num, int numX, int numY) {

	int destX = (num - 1) / rows;
	int destY = (num - 1) % rows;


	if (finalZeroPosition != -1 && num > finalZeroPosition) {
		destX = num / rows;
		destY = num % rows;
	}

	return (abs(numX - destX) + abs(numY - destY));
}

unsigned int heuristicValue(unsigned int** matrix) {
	unsigned int hValue = 0;

	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < rows; ++j) {
			unsigned int num = matrix[i][j];
			if (num != 0) {
				hValue += getManhattonDistance(num, i, j);
			}
		}
	}

	return hValue;
}

void swapTiles(unsigned int** matrix, vector<Tile> path) {

	int nextX = path.back().getX(), nextY = path.back().getY();
	path.pop_back();
	int currentX = path.back().getX(), currentY = path.back().getY();

	int temp = matrix[currentX][currentY];
	matrix[currentX][currentY] = matrix[nextX][nextY];
	matrix[nextX][nextY] = temp;
}

int slideTiles(unsigned int** matrix, int g, int& threshold, vector<Tile>& path, Tile prev) {

	int h = heuristicValue(matrix);
	int f = g + h;

	if (f > threshold)
		return f;

	if (h == 0) { 
		return -1; 
	}

	int min = INT_MAX;

	Tile current = path.back();
	for (Tile next : generateAdjTiles(current, prev)) {
		path.push_back(next);
		prev = current;
		swapTiles(matrix, path);
		int temp = slideTiles(matrix, g + 1, threshold, path, prev);

		if (temp == -1)
			return -1;

		if (temp < min)
			min = temp;

		swapTiles(matrix, path);
		path.pop_back();
	}

	return min;
}

vector<Tile> solvePuzzle(unsigned int** matrix, Tile initialEmptyTile) { 
	vector<Tile> currentPath;
	currentPath.push_back(initialEmptyTile); 

	int threshold = heuristicValue(matrix);

	while (true) {

		int temp = slideTiles(matrix, 0, threshold, currentPath, Tile(-1, -1));

		if (temp == -1)
			return currentPath;

		//if (temp == INT_MAX)
			//return vector<Tile>();

		threshold = temp;
	}
}

string getDirection(Tile& current, Tile& next) {
	int currentX = current.getX(), currentY = current.getY();
	int nextX = next.getX(), nextY = next.getY();

	if (currentX > nextX)
		return "down";
	else if (currentX < nextX)
		return "up";
	else if (currentY > nextY)
		return "right";
	else
		return "left";
}

static int getInvCount(const vector<int> &arr) {
	int inv_count = 0;
	for (int i = 0; i < arr.size() - 1; i++)
		for (int j = i + 1; j < arr.size(); j++)
			if (arr[i] > arr[j])
				inv_count++;

	return inv_count;
}

bool isSolvable(int boardSqSize, unsigned int **A) {

	int blank_row_no = -1;
	vector<int> arr;
	for (int i = 0; i < boardSqSize; i++) {
		for (int j = 0; j < boardSqSize; j++) {
			if (A[i][j])
				arr.push_back(A[i][j]);
			else
				blank_row_no = i / boardSqSize;
		}
	}
	int invCount = getInvCount(arr);
	bool boardSizeOdd = static_cast<bool>(boardSqSize & 1);
	if (boardSizeOdd && !(invCount & 1))
		return true;
	else if (!boardSizeOdd && ((blank_row_no + getInvCount(arr)) & 1))
		return true;
	return false;
}

int main() 
{
	unsigned int N;
	int zeroX = 0, zeroY = 0;
	Tile* initialEmptyTile = new Tile(0, 0);

	cout << "Enter value for N: ";
	cin >> N;

	rows = (unsigned int)sqrt(N + 1);
	unsigned int** initialMatrix = new unsigned int*[rows];

	for (unsigned int i = 0; i < rows; ++i) {
		initialMatrix[i] = new unsigned int[rows];
	}

	cout << "Enter the Zero position: ";
	cin >> finalZeroPosition; 
	

	cout << "Enter the puzzle: " << endl;
	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < rows; ++j) {
			cin >> initialMatrix[i][j];
			if (initialMatrix[i][j] == 0) {
				initialEmptyTile = new Tile(i, j);
			}
		}
	}

	if (!isSolvable(rows, initialMatrix)) {
		cout << "No Solution" << endl;
		system("pause");
		return 0;
	}

	vector<Tile> resultPath = solvePuzzle(initialMatrix, *initialEmptyTile);
	
	cout << endl;
	cout << resultPath.size() - 1 << endl; 

	for (unsigned int i = 0; i < resultPath.size() - 1; ++i) {
		cout << getDirection(resultPath.at(i), resultPath.at(i + 1)) << endl;
	}
	system("pause");
	return 0;
}