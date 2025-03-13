#include "HeaderFiles/Vector2Int.h"
#include "HeaderFiles/Vector2.h"

Vector2Int::Vector2Int() : X(0), Y(0) {/*empty*/ }
Vector2Int::Vector2Int(int x, int y) : X(x), Y(y) {/*empty*/ }

Vector2Int::operator Vector2() const
{
	return Vector2(X, Y);
}

int Vector2Int::GetMagnitudeSqrd(Vector2Int v) { return v.X * v.X + v.Y * v.Y; }
int Vector2Int::GetMagnitudeSqrd() { return GetMagnitudeSqrd(*this); }

Vector2Int Vector2Int::Normalize() { return *this / sqrt(GetMagnitudeSqrd()); }

bool Vector2Int::operator==(const Vector2Int& v)
{
	return this->X == v.X && this->Y == v.Y;
}

bool Vector2Int::operator<(const Vector2Int& v)
{
	return this->X < v.X && this->Y < v.Y;
}
bool Vector2Int::operator>(const Vector2Int& v)
{
	return this->X > v.X && this->Y > v.Y;
}

Vector2Int Vector2Int::operator +(Vector2Int other)
{
	return Vector2Int(X + other.X, Y + other.Y);
}
Vector2Int Vector2Int::operator -(Vector2Int other)
{
	return Vector2Int(X - other.X, Y - other.Y);
}
Vector2Int Vector2Int::operator *(Vector2Int other)
{
	return Vector2Int(X * other.X, Y * other.Y);
}
Vector2Int Vector2Int::operator /(Vector2Int other)
{
	return Vector2Int(X / other.X, Y / other.Y);
}

Vector2Int Vector2Int::operator *(double other)
{
	return Vector2Int(X * other, Y * other);
}
Vector2Int Vector2Int::operator /(double other)
{
	return Vector2Int(X / other, Y / other);
}

Vector2Int& Vector2Int::operator +=(const Vector2Int& other)
{
	this->X += other.X;
	this->Y += other.Y;
	return *this;
}
Vector2Int& Vector2Int::operator -=(const Vector2Int& other)
{
	this->X -= other.X;
	this->Y -= other.Y;
	return *this;
}
Vector2Int& Vector2Int::operator *=(const Vector2Int& other)
{
	this->X *= other.X;
	this->Y *= other.Y;
	return *this;
}
Vector2Int& Vector2Int::operator /=(const Vector2Int& other)
{
	this->X /= other.X;
	this->Y /= other.Y;
	return *this;
}