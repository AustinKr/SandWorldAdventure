#include "HeaderFiles/Vector2Hasher.h"
#include "HeaderFiles/Vector2.h"
#include "HeaderFiles/Vector2Int.h"

namespace SandboxEngine
{
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
}