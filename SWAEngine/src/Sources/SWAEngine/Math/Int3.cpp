#pragma once
#include "SWAEngine/Math/Int3.h"
#include <algorithm>

namespace SWAEngine::Math
{
	bool Int3::operator==(const Int3& v) const
	{
		return X == v.X && Y == v.Y && Z == v.Z;
	}
	bool Int3::operator<(const Int3& v) const
	{
		return X < v.X && Y < v.Y && Z < v.Z;
	}
	bool Int3::operator>(const Int3& v) const
	{
		return X > v.X && Y > v.Y && Z > v.Z;
	}

	// Int3Hasher
	std::size_t Int3Hasher::operator()(const Int3& v) const
	{
		return std::hash<int>()(v.X) ^
			(std::hash<int>()(v.Y) << 1) ^
			(std::hash<int>()(v.Z) << 2);
	}

	// Int3Compare
	bool Int3Compare::operator()(const Int3& a, const Int3& b) const
	{
		return Int3Hasher().operator()(a) < Int3Hasher().operator()(b);
	}
}