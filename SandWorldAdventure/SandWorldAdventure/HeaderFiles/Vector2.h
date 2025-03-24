#pragma once
#include <iostream>

#define VECTOR2_H

namespace SandboxEngine
{
#ifndef VECTOR2INT_H
#define VECTOR2INT_H
	struct Vector2Int;
#endif

	struct Vector2
	{
		double X;
		double Y;

		Vector2();
		Vector2(double x, double y);

		operator Vector2Int() const;

		static double GetMagnitudeSqrd(Vector2 v);
		double GetMagnitudeSqrd();
		Vector2 Normalize();
		static Vector2 Normalize(Vector2 v);

		bool operator==(const Vector2& v) const;
		bool operator<(const Vector2& v);
		bool operator>(const Vector2& v);
		//friend bool operator==(const Vector2& v1, const Vector2& v2);

		//friend bool operator<(const Vector2& v1, const Vector2& v2);
		//friend bool operator>(const Vector2& v1, const Vector2& v2);

		Vector2 operator +(Vector2 other);
		Vector2 operator -(Vector2 other);
		Vector2 operator *(Vector2 other);
		Vector2 operator /(Vector2 other);

		Vector2 operator *(double other);
		Vector2 operator /(double other);

		Vector2& operator +=(const Vector2& other);
		Vector2& operator -=(const Vector2& other);
		Vector2& operator *=(const Vector2& other);
		Vector2& operator /=(const Vector2& other);

	};
}