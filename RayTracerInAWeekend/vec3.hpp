#pragma once
#include <iostream>

using std::sqrt;

class vec3 {
public:
	float x;
	float y;
	float z;
	vec3() : x{ 0 }, y{ 0 }, z{ 0 } {}
	vec3(float e) : x{ e }, y{ e }, z{ e } {}
	vec3(float e0, float e1, float e2) : x{ e0 }, y{ e1 }, z{ e2 } {}

	vec3 operator-() const { return vec3(-x, -y, -z); }
	float operator[](int i) const { return (0 ? x : 1 ? y : z); }
	float& operator[](int i) { return (0 ? x : 1 ? y : z); }

	vec3& operator+=(const vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec3 operator*=(float t) {
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}

	vec3& operator/=(float t) {
		x *= 1/t;
		y *= 1/t;
		z *= 1/t;
		return *this;
	}
	float length() const {
		return sqrt(length_squared());
	}
	float length_squared() const {
		return x * x + y * y + z * z;
	}
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.x << ' ' << v.y << ' ' << v.z;
}
inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}
inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}
inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}
inline vec3 operator*(float t, const vec3& v) {
	return vec3(t * v.x, t * v.y, t * v.z);
}
inline vec3 operator*(const vec3& v, float t) {
	return t * v;
}
inline vec3 operator/(const vec3& v, float t) {
	return (1 / t) * v;
}
inline float dot(const vec3& u, const vec3& v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}
inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y-u.y*v.x);
}
inline vec3 unit_vector(const  vec3& v) {
	return v / v.length();
}