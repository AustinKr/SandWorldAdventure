#pragma once

namespace SandboxEngine::GraphicsPipeline
{
	// Types that are used within the shader code
	struct float2
	{
		float X;
		float Y;

		inline float2 operator *(float other)
		{
			return float2(other * X, other * Y);
		}
		inline float2 operator /(float other)
		{
			return (*this) * (1.0f / other);
		}
		inline float2 operator *(float2 other)
		{
			return float2(other.X * X, other.Y * Y);
		}
		inline float2 operator /(float2 other)
		{
			return float2(X / other.X, Y / other.Y);
		}

		inline float2 operator +(float2 other)
		{
			return float2(other.X + X, other.Y + Y);
		}
		inline float2 operator -(float2 other)
		{
			return float2(-other.X + X, -other.Y + Y);
		}

		inline void operator +=(float2 other)
		{
			X += other.X;
			Y += other.Y;
		}
		inline void operator -=(float2 other)
		{
			X -= other.X;
			Y -= other.Y;
		}
		inline void operator *=(float2 other)
		{
			X *= other.X;
			Y *= other.Y;
		}
		inline void operator /=(float2 other)
		{
			X /= other.X;
			Y /= other.Y;
		}
	};
	struct float3
	{
		float X;
		float Y;
		float Z;

		inline float3 operator *(float other)
		{
			return float3(other * X, other * Y, other * Z);
		}
		inline float3 operator /(float other)
		{
			return (*this) * (1.0f/other);
		}
		inline float3 operator *(float3 other)
		{
			return float3(other.X * X, other.Y * Y, other.Z * Z);
		}
		inline float3 operator /(float3 other)
		{
			return float3(X / other.X, Y / other.Y, Z / other.Z);
		}

		inline float3 operator +(float3 other)
		{
			return float3(other.X + X, other.Y + Y, other.Z + Z);
		}
		inline float3 operator -(float3 other)
		{
			return float3(-other.X + X, -other.Y + Y, -other.Z + Z);
		}

		inline void operator +=(float3 other)
		{
			X += other.X;
			Y += other.Y;
			Z += other.Z;
		}
		inline void operator -=(float3 other)
		{
			X -= other.X;
			Y -= other.Y;
			Z -= other.Z;
		}
		inline void operator *=(float3 other)
		{
			X *= other.X;
			Y *= other.Y;
			Z *= other.Z;
		}
		inline void operator /=(float3 other)
		{
			X /= other.X;
			Y /= other.Y;
			Z /= other.Z;
		}
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