#pragma once
#define M_PI 3.14159265
#define RAD_TO_DEG(x) x * (180.f / M_PI)
#define DEG_TO_RAD(x) x * (M_PI / 180.f)

struct RGB
{
	float R, G, B;
};

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

struct vec3
{
	float x, y, z;

	vec3 operator-(vec3 ape)
	{
		return { x - ape.x, y - ape.y, z - ape.z };
	}

	vec3 operator+(vec3 ape)
	{
		return { x + ape.x, y + ape.y, z + ape.z };
	}

	vec3 operator*(float ape)
	{
		return { x * ape, y * ape, z * ape };
	}

	vec3 operator/(float ape)
	{
		return { x / ape, y / ape, z / ape };
	}

	vec3 operator/=(float ape)
	{
		x /= ape;
		y /= ape;
		z /= ape;

		return *this;
	}

	vec3 operator+=(vec3 ape)
	{
		return { x += ape.x, y += ape.y, z += ape.z };
	}

	vec3 operator-=(vec3 ape)
	{
		return { x -= ape.x, y -= ape.y, z -= ape.z };
	}

	vec3 TransformVec(vec3 angle, float ape)
	{
		vec3 ret;
		ret.x = x + cosf(DEG_TO_RAD(angle.y)) * ape;
		ret.y = y + sinf(DEG_TO_RAD(angle.y)) * ape;
		ret.z = z - tanf(DEG_TO_RAD(angle.x)) * ape;
		return ret;
	}

	void Normalize()
	{
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}

	float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float DistTo(vec3 ape)
	{
		return (*this - ape).Length();
	}

	float Dist2D(vec3 ape)
	{
		return (*this - ape).Length2D();
	}
};
