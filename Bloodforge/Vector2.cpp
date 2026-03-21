#include "pch.h"
#include "Vector2.h"

namespace Bloodforge
{
    Vector2 Vector2::operator+(const Vector2& obj)
    {
        return
        {
            X = X + obj.X,
            Y = Y + obj.Y
        };
    }

    Vector2 Vector2::operator-(const Vector2& obj)
    {
        return
        {
            X = X - obj.X,
            Y = Y - obj.Y
        };
    }

    Vector2 Vector2::operator*(float scalar)
    {
        return
        {
            X *= scalar,
            Y *= scalar,
        };
    }
    Vector2 Vector2::operator/(float scalar)
    {
        return
        {
            X /= scalar,
            Y /= scalar,
        };
    }
}