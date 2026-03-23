#include "pch.h"
#include "Vector2.h"

namespace Bloodforge
{
    Vector2 Vector2::operator+(const Vector2& obj) const
    {
        return
        {
            X + obj.X,
            Y + obj.Y
        };
    }

    Vector2 Vector2::operator-(const Vector2& obj) const
    {
        return
        {
            X - obj.X,
            Y - obj.Y
        };
    }

    Vector2 Vector2::operator*(float scalar) const
    {
        return
        {
            X *scalar,
            Y *scalar,
        };
    }
    Vector2 Vector2::operator/(float scalar) const
    {
        return
        {
            X / scalar,
            Y / scalar,
        };
    }
}