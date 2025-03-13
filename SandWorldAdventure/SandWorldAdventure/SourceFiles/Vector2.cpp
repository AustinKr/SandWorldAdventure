#include "HeaderFiles/Vector2.h"
#include "HeaderFiles/Vector2Int.h"

namespace SandboxEngine
{
	Vector2::Vector2() : X(0), Y(0) {/*empty*/ }
	Vector2::Vector2(double x, double y) : X(x), Y(y) {/*empty*/ }

	Vector2::operator Vector2Int() const
	{
		return Vector2Int(X, Y);
	}

	double Vector2::GetMagnitudeSqrd(Vector2 v) { return v.X * v.X + v.Y * v.Y; }
	double Vector2::GetMagnitudeSqrd() { return GetMagnitudeSqrd(*this); }

	Vector2 Vector2::Normalize() { return *this / sqrt(GetMagnitudeSqrd()); }

	bool Vector2::operator==(const Vector2& v) const
	{
		return this->X == v.X && this->Y == v.Y;
	}

	bool Vector2::operator<(const Vector2& v)
	{
		return this->X < v.X && this->Y < v.Y;
	}
	bool Vector2::operator>(const Vector2& v)
	{
		return this->X > v.X && this->Y > v.Y;
	}

	//Vector2::operator==(const Vector2& v1, const Vector2& v2)
	//{
	//	return v1.X == v2.X && v1.Y == v2.Y;
	//}
	//
	//bool Vector2::operator<(const Vector2& v1, const Vector2& v2)
	//{
	//	return v1.X < v2.X && v1.Y < v2.Y;
	//}
	//bool Vector2::operator>(const Vector2& v1, const Vector2& v2)
	//{
	//	return v1.X > v2.X && v1.Y > v2.Y;
	//}

	Vector2 Vector2::operator +(Vector2 other)
	{
		return Vector2(X + other.X, Y + other.Y);
	}
	Vector2 Vector2::operator -(Vector2 other)
	{
		return Vector2(X - other.X, Y - other.Y);
	}
	Vector2 Vector2::operator *(Vector2 other)
	{
		return Vector2(X * other.X, Y * other.Y);
	}
	Vector2 Vector2::operator /(Vector2 other)
	{
		return Vector2(X / other.X, Y / other.Y);
	}

	Vector2 Vector2::operator *(double other)
	{
		return Vector2(X * other, Y * other);
	}
	Vector2 Vector2::operator /(double other)
	{
		return Vector2(X / other, Y / other);
	}

	Vector2& Vector2::operator +=(const Vector2& other)
	{
		this->X += other.X;
		this->Y += other.Y;
		return *this;
	}
	Vector2& Vector2::operator -=(const Vector2& other)
	{
		this->X -= other.X;
		this->Y -= other.Y;
		return *this;
	}
	Vector2& Vector2::operator *=(const Vector2& other)
	{
		this->X *= other.X;
		this->Y *= other.Y;
		return *this;
	}
	Vector2& Vector2::operator /=(const Vector2& other)
	{
		this->X /= other.X;
		this->Y /= other.Y;
		return *this;
	}
}