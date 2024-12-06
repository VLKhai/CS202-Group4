#pragma once

class Vector2
{
private:
	int x, y;
public:
	Vector2() = default;
	Vector2(int x, int y);
	~Vector2();

	int getX();
	int getY();
	void setX(int x);
	void setY(int y);
};