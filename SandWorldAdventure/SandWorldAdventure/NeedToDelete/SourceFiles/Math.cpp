#include "HeaderFiles/Math.h"

namespace SandboxEngine
{
	// Vector2

	Vector2::Vector2() : X(0), Y(0) {/*empty*/ }
	Vector2::Vector2(double x, double y) : X(x), Y(y) {/*empty*/ }

	Vector2::operator Vector2Int() const
	{
		return Vector2Int(X, Y);
	}
	Vector2::operator float2() const
	{
		return float2(X, Y);
	}

	double Vector2::GetMagnitudeSqrd(Vector2 v) { return v.X * v.X + v.Y * v.Y; }
	double Vector2::GetMagnitudeSqrd() const { return GetMagnitudeSqrd(*this); }

	Vector2 Vector2::Normalize() const { return GetMagnitudeSqrd() == 0 ? Vector2(0, 0) : (*this / sqrt(GetMagnitudeSqrd())); }
	Vector2 Vector2::Normalize(Vector2 v) { return v.Normalize(); }

	bool Vector2::operator==(const Vector2& v) const
	{
		return this->X == v.X && this->Y == v.Y;
	}

	bool Vector2::operator<(const Vector2& v) const
	{
		return this->X < v.X && this->Y < v.Y;
	}
	bool Vector2::operator>(const Vector2& v) const
	{
		return this->X > v.X && this->Y > v.Y;
	}

	Vector2 Vector2::operator +(Vector2 other) const
	{
		return Vector2(X + other.X, Y + other.Y);
	}
	Vector2 Vector2::operator -(Vector2 other) const
	{
		return Vector2(X - other.X, Y - other.Y);
	}
	Vector2 Vector2::operator *(Vector2 other) const
	{
		return Vector2(X * other.X, Y * other.Y);
	}
	Vector2 Vector2::operator /(Vector2 other) const
	{
		return Vector2(X / other.X, Y / other.Y);
	}

	Vector2 Vector2::operator *(double other) const
	{
		return Vector2(X * other, Y * other);
	}
	Vector2 Vector2::operator /(double other) const
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

	// Vector2Int

	Vector2Int::Vector2Int() : X(0), Y(0) {/*empty*/ }
	Vector2Int::Vector2Int(int x, int y) : X(x), Y(y) {/*empty*/ }

	Vector2Int::operator Vector2() const
	{
		return Vector2(X, Y);
	}
	Vector2Int::operator float2() const
	{
		return float2(X, Y);
	}

	int Vector2Int::GetMagnitudeSqrd(Vector2Int v) { return v.X * v.X + v.Y * v.Y; }
	int Vector2Int::GetMagnitudeSqrd() const { return GetMagnitudeSqrd(*this); }

	Vector2Int Vector2Int::Normalize() const { return *this / sqrt(GetMagnitudeSqrd()); }

	bool Vector2Int::operator==(const Vector2Int& v) const
	{
		return this->X == v.X && this->Y == v.Y;
	}

	bool Vector2Int::operator<(const Vector2Int& v) const
	{
		return this->X < v.X && this->Y < v.Y;
	}
	bool Vector2Int::operator>(const Vector2Int& v) const
	{
		return this->X > v.X && this->Y > v.Y;
	}

	Vector2Int Vector2Int::operator +(Vector2Int other) const
	{
		return Vector2Int(X + other.X, Y + other.Y);
	}
	Vector2Int Vector2Int::operator -(Vector2Int other) const
	{
		return Vector2Int(X - other.X, Y - other.Y);
	}
	Vector2Int Vector2Int::operator *(Vector2Int other) const
	{
		return Vector2Int(X * other.X, Y * other.Y);
	}
	Vector2Int Vector2Int::operator /(Vector2Int other) const
	{
		return Vector2Int(X / other.X, Y / other.Y);
	}

	// TODO: THese should be ints
	Vector2Int Vector2Int::operator *(double other) const
	{
		return Vector2Int(X * other, Y * other);
	}
	Vector2Int Vector2Int::operator /(double other) const
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

	// Vector2Hasher

	std::size_t Vector2Hasher::operator()(const Vector2& v) const
	{
		return std::hash<double>()(v.X) ^
			(std::hash<double>()(v.Y) << 1);
	}
	std::size_t Vector2Hasher::operator()(const Vector2Int& v) const
	{
		return std::hash<int>()(v.X) ^
			(std::hash<int>()(v.Y) << 1);
	}

	// Vector2Compare
	bool Vector2Compare::operator()(const Vector2& a, const Vector2& b) const
	{
		return Vector2Hasher().operator()(a) < Vector2Hasher().operator()(b);
	}
	bool Vector2Compare::operator()(const Vector2Int& a, const Vector2Int& b) const
	{
		return Vector2Hasher().operator()(a) < Vector2Hasher().operator()(b);
	}

	// float2

	float2::operator Vector2() const
	{
		return Vector2(X, Y);
	}
	float2::operator Vector2Int() const
	{
		return Vector2Int(X, Y);
	}

	float2 float2::operator *(float other) const
	{
		return float2(other * X, other * Y);
	}
	float2 float2::operator /(float other) const
	{
		return (*this) * (1.0f / other);
	}
	float2 float2::operator *(float2 other) const
	{
		return float2(other.X * X, other.Y * Y);
	}
	float2 float2::operator /(float2 other) const
	{
		return float2(X / other.X, Y / other.Y);
	}

	float2 float2::operator +(float2 other) const
	{
		return float2(other.X + X, other.Y + Y);
	}
	float2 float2::operator -(float2 other) const
	{
		return float2(-other.X + X, -other.Y + Y);
	}

	void float2::operator +=(float2 other)
	{
		X += other.X;
		Y += other.Y;
	}
	void float2::operator -=(float2 other)
	{
		X -= other.X;
		Y -= other.Y;
	}
	void float2::operator *=(float2 other)
	{
		X *= other.X;
		Y *= other.Y;
	}
	void float2::operator /=(float2 other)
	{
		X /= other.X;
		Y /= other.Y;
	}

	// float3

	float3 float3::operator *(float other)
	{
		return float3(other * X, other * Y, other * Z);
	}
	float3 float3::operator /(float other)
	{
		return (*this) * (1.0f / other);
	}
	float3 float3::operator *(float3 other)
	{
		return float3(other.X * X, other.Y * Y, other.Z * Z);
	}
	float3 float3::operator /(float3 other)
	{
		return float3(X / other.X, Y / other.Y, Z / other.Z);
	}

	float3 float3::operator +(float3 other)
	{
		return float3(other.X + X, other.Y + Y, other.Z + Z);
	}
	float3 float3::operator -(float3 other)
	{
		return float3(-other.X + X, -other.Y + Y, -other.Z + Z);
	}

	void float3::operator +=(float3 other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
	}
	void float3::operator -=(float3 other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
	}
	void float3::operator *=(float3 other)
	{
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
	}
	void float3::operator /=(float3 other)
	{
		X /= other.X;
		Y /= other.Y;
		Z /= other.Z;
	}
}