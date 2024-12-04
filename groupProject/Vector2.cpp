#include "Vector2.h"

Vector2::Vector2() {}

Vector2::Vector2(int x, int y) : x(x), y(y) {}

Vector2::~Vector2() {}

int Vector2::getX() { return x; }

int Vector2::getY() { return y; }

void Vector2::setX(int x) { this->x = x; }

void Vector2::setY(int y) { this->y = y; }