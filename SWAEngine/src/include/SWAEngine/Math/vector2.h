#pragma once
#include "SWAEngine/dllClause.h"

namespace SWAEngine::Math
{
	// Forward declarations
	struct SWA_ENGINE_API Vector2Int;

	struct SWA_ENGINE_API Vector2
	{
		double X;
		double Y;

		Vector2();
		Vector2(double x, double y);

		operator Vector2Int() const;

		static double GetMagnitudeSqrd(Vector2 v);
		double GetMagnitudeSqrd() const;
		Vector2 Normalize() const;
		static Vector2 Normalize(Vector2 v);

		bool operator==(const Vector2& v) const;
		bool operator<(const Vector2& v) const;
		bool operator>(const Vector2& v) const;

		Vector2 operator +(Vector2 other) const;
		Vector2 operator -(Vector2 other) const;
		Vector2 operator *(Vector2 other) const;
		Vector2 operator /(Vector2 other) const;

		Vector2 operator *(double other) const;
		Vector2 operator /(double other) const;

		Vector2& operator +=(const Vector2& other);
		Vector2& operator -=(const Vector2& other);
		Vector2& operator *=(const Vector2& other);
		Vector2& operator /=(const Vector2& other);

		Vector2& operator *=(const double other);
		Vector2& operator /=(const double other);

	};
	struct SWA_ENGINE_API Vector2Int
	{
		int X;
		int Y;

		Vector2Int();
		Vector2Int(int x, int y);

		operator Vector2() const;

		static int GetMagnitudeSqrd(Vector2Int v);
		int GetMagnitudeSqrd() const;

		Vector2Int Normalize() const;

		bool operator==(const Vector2Int& v) const;
		bool operator<(const Vector2Int& v) const;
		bool operator>(const Vector2Int& v) const;

		Vector2Int operator +(Vector2Int other) const;
		Vector2Int operator -(Vector2Int other) const;
		Vector2Int operator *(Vector2Int other) const;
		Vector2Int operator /(Vector2Int other) const;

		Vector2Int operator *(double other) const;
		Vector2Int operator /(double other) const;

		Vector2Int& operator +=(const Vector2Int& other);
		Vector2Int& operator -=(const Vector2Int& other);
		Vector2Int& operator *=(const Vector2Int& other);
		Vector2Int& operator /=(const Vector2Int& other);

	};

	struct SWA_ENGINE_API Vector2Hasher
	{
		unsigned long long operator()(const Vector2& v) const;
		unsigned long long operator()(const Vector2Int& v) const;
	};
	struct SWA_ENGINE_API Vector2Compare
	{
		bool operator()(const Vector2& a, const Vector2& b) const;
		bool operator()(const Vector2Int& a, const Vector2Int& b) const;
	};
}