#pragma once
#include <iostream>
//#include "HeaderFiles/Vector2Int.h"
//#include "HeaderFiles/Vector2.h"

namespace SandboxEngine
{
#ifndef VECTOR2INT_H
#define VECTOR2INT_H
    struct Vector2Int;
#endif
#ifndef VECTOR2_H
#define VECTOR2_H
    struct Vector2;
#endif

    struct Vector2Hasher
    {
        std::size_t operator()(const Vector2& v) const;
        std::size_t operator()(const Vector2Int& v) const;
    };
}