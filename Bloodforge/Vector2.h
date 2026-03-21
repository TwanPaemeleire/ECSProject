#pragma once
namespace Bloodforge
{
	struct Vector2
	{
		float X = 0.0f;
		float Y = 0.0f;

		Vector2 operator+ (const Vector2 & obj);
		Vector2 operator- (const Vector2 & obj);
		Vector2 operator* (float scalar);
		Vector2 operator/ (float scalar);
	};
}