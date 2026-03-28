#pragma once
#include "SWAEngine/Math/Int3.h"

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
}