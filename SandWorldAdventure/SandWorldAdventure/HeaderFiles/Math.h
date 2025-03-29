#pragma once
#include <iostream>
//#define MATH_H;
//
//#ifndef VECTOR2_H
//#define VECTOR2_H
//struct SandboxEngine::Vector2;
//#endif

namespace SandboxEngine
{
	// Forward declarations
	struct Vector2Int;
	struct float2;

	struct Vector2
	{
		double X;
		double Y;

		Vector2();
		Vector2(double x, double y);

		operator Vector2Int() const;
		operator float2() const;

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

	};
	struct Vector2Int
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

	struct Vector2Hasher
	{
		std::size_t operator()(const Vector2& v) const;
		std::size_t operator()(const Vector2Int& v) const;
	};

	// Types that are used within the shader code
	struct float2
	{
		float X;
		float Y;

		operator Vector2() const;
		
		float2 operator *(float other) const;
		float2 operator /(float other) const;
		float2 operator *(float2 other) const;
		float2 operator /(float2 other) const;

		float2 operator +(float2 other) const;
		float2 operator -(float2 other) const;

		void operator +=(float2 other);
		void operator -=(float2 other);
		void operator *=(float2 other);
		void operator /=(float2 other);
	};
	struct float3
	{
		float X;
		float Y;
		float Z;

		float3 operator *(float other);
		float3 operator /(float other);
		float3 operator *(float3 other);
		float3 operator /(float3 other);

		float3 operator +(float3 other);
		float3 operator -(float3 other);

		void operator +=(float3 other);
		void operator -=(float3 other);
		void operator *=(float3 other);
		void operator /=(float3 other);
	};
	struct float4
	{
		float X;
		float Y;
		float Z;
		float W;
	};

	struct Vertex
	{
		float2 pos;
		float3 uvCoord;
	};
}