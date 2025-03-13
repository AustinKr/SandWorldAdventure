#pragma once
#include <iostream>

#define VECTOR2INT_H
namespace SandboxEngine
{
#ifndef VECTOR2_H
#define VECTOR2_H
	struct Vector2;
#endif

	struct Vector2Int
	{
		int X;
		int Y;

		Vector2Int();
		Vector2Int(int x, int y);

		operator Vector2() const;

		static int GetMagnitudeSqrd(Vector2Int v);
		int GetMagnitudeSqrd();

		Vector2Int Normalize();
		
		bool operator==(const Vector2Int& v) const;
		bool operator<(const Vector2Int& v);
		bool operator>(const Vector2Int& v);
		/*friend bool operator==(const Vector2Int& v1, const Vector2Int& v2)
		{
			return v1.X == v2.X && v1.Y == v2.Y;
		}

		friend bool operator<(const Vector2Int& v1, const Vector2Int& v2)
		{
			return v1.X < v2.X && v1.Y < v2.Y;
		}
		friend bool operator>(const Vector2Int& v1, const Vector2Int& v2)
		{
			return v1.X > v2.X && v1.Y > v2.Y;
		}*/

		Vector2Int operator +(Vector2Int other);
		Vector2Int operator -(Vector2Int other);
		Vector2Int operator *(Vector2Int other);
		Vector2Int operator /(Vector2Int other);

		Vector2Int operator *(double other);
		Vector2Int operator /(double other);

		Vector2Int& operator +=(const Vector2Int& other);
		Vector2Int& operator -=(const Vector2Int& other);
		Vector2Int& operator *=(const Vector2Int& other);
		Vector2Int& operator /=(const Vector2Int& other);

	};
}