/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include <windows.h>

typedef DWORD Void;
typedef DWORD Any;
typedef DWORD uint;
typedef DWORD Hash;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int ScrHandle;

#pragma pack(push, 1)
typedef struct Vector3
{
	float x;
	DWORD _paddingx;
	float y;
	DWORD _paddingy;
	float z;
	DWORD _paddingz;

	constexpr Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z), _paddingx(0), _paddingy(0), _paddingz(0) {}
	bool operator==(Vector3& v) const { return (v.x == x && v.y == y && v.z == z); }
	bool operator!=(Vector3& v) const { return (v.x != x || v.y != y || v.z != z); }
	Vector3 operator-() const { return Vector3(-x, -y, -z); }
	Vector3 operator*(const float f) const { return Vector3(f*x, f*y, f*z); }
} Vector3;
#pragma pack(pop)