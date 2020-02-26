#pragma once
#include <string>

class Tile
{
private:
	int x;
	int y;

public:

	Tile(int, int);
	Tile(const Tile&);
	~Tile();

	Tile& operator= (Tile);

	friend bool operator==(const Tile&, const Tile&);
	friend bool operator!=(const Tile&, const Tile&);

	int getX();
	int getY();

};