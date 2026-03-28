#pragma once
#include "SWAEngine/dllClause.h"

namespace SWAEngine::Math
{
	struct SWA_ENGINE_API Int3
	{
		int X;
		int Y;
		int Z;

		bool operator==(const Int3& v) const;
		bool operator<(const Int3& v) const;
		bool operator>(const Int3& v) const;
	};
}