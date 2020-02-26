#include "Tile.h"

using namespace std;

Tile::Tile(int x, int y) : x(x), y(y) {
}

Tile::Tile(const Tile& other) : x(other.x), y(other.y) {
}

Tile::~Tile() {
}


Tile& Tile::operator= (Tile other) {
	swap(x, other.x); 
	swap(y, other.y);
	return *this;
}

bool operator==(const Tile& lhs, const Tile& rhs) {
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

bool operator!=(const Tile& lhs, const Tile& rhs) {
	return !(lhs == rhs);
}

int Tile::getX() {
	return this->x;
}

int Tile::getY() {
	return this->y;
}