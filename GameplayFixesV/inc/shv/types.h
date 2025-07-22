/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include <windows.h>
#include <math.h>

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
    // Align to 8 bytes because these floats should be a "scrValue" union (with size "unsigned long long")
    alignas(8) float x;
    alignas(8) float y;
    alignas(8) float z;

    // Constructors
    constexpr Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    constexpr Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    constexpr Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

    // Assignment operator
    constexpr Vector3& operator=(const Vector3& v) {
        if (this != &v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }

    // Vector Addition (Vector + Vector)
    constexpr Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    // Vector Subtraction (Vector - Vector)
    constexpr Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    // Scalar Multiplication (Vector * float)
    constexpr Vector3 operator*(float f) const {
        return Vector3(x * f, y * f, z * f);
    }

    // Scalar Division (Vector / float)
    constexpr Vector3 operator/(float f) const {
        if (f != 0.0f)
            return Vector3(x / f, y / f, z / f);

        return Vector3();
    }

    // Unary Negation (-Vector)
    constexpr Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    // Vector Addition Assignment (Vector += Vector)
    constexpr Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    // Vector Subtraction Assignment (Vector -= Vector)
    constexpr Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    // Vector Multiplication Assignment (Vector *= Vector)
    constexpr Vector3& operator*=(const Vector3& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    // Vector Division Assignment (Vector /= Vector)
    constexpr Vector3& operator/=(const Vector3& v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    // Scalar Addition Assignment (Vector += float)
    constexpr Vector3& operator+=(float f) {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    // Scalar Subtraction Assignment (Vector -= float)
    constexpr Vector3& operator-=(float f) {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    // Scalar Multiplication Assignment (Vector *= float)
    constexpr Vector3& operator*=(float f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    // Scalar Division Assignment (Vector /= float)
    constexpr Vector3& operator/=(float f) {
        if (f != 0.0f) {
            x /= f;
            y /= f;
            z /= f;
        }
        return *this;
    }

    // Equality (Vector == Vector)
    constexpr bool operator==(const Vector3& v) const {
        return (v.x == x && v.y == y && v.z == z);
    }

    // Inequality (Vector != Vector)
    constexpr bool operator!=(const Vector3& v) const {
        return !(*this == v);
    }

    // Dot Product (Vector . Vector)
    constexpr float Dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Cross Product (Vector x Vector)
    constexpr Vector3 Cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    // Magnitude/Length of the vector
    float Length() const {
        return static_cast<float>(sqrt(x * x + y * y + z * z));
    }

    // Squared Magnitude/Length
    float LengthSq() const {
        return x * x + y * y + z * z;
    }

    // Normalization (returns a new normalized vector)
    Vector3 Normalize() const {
        const float len = Length();
        if (len > 0.0f) {
            return *this / len;
        }
        return Vector3();
    }
} Vector3;
#pragma pack(pop)