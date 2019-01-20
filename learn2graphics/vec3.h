#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>

//Vector 3 with supporting operators and matrix math functions
//Float based
class vec3
{
public:
	//literally just 3 floats in an array, but we want to do a lot of math
	//on these three floats
	float e[3];

	//constructors
	vec3() {}
	vec3(float e0, float e1, float e2) {e[0] = e0; e[1] = e1; e[2] = e2;}

	//handy accessors for either color or vector usage
	inline float x() const {return e[0];}
	inline float y() const {return e[1];}
	inline float z() const {return e[2];}
	inline float r() const {return e[0];}
	inline float g() const {return e[1];}
	inline float b() const {return e[2];}

	//unary plus/minus ie when you go -5.1f or +5.1f (indicating positivity or negatvity of value)
	inline const vec3& operator+() const {return *this;}
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	//like array accessors
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	//operators expressed in impl
	inline vec3& operator+=(const vec3 &v2);
	inline vec3& operator-=(const vec3 &v2);
	inline vec3& operator*=(const vec3 &v2);
	inline vec3& operator/=(const vec3 &v2);
	inline vec3& operator*=(const float t);
	inline vec3& operator/=(const float t);

	//common linear algebra functions
	inline float length() const 
	{
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); 
	}
	inline float squared_length() const 
	{
		return  e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void make_unit_vector();
};

inline std::istream& operator>> (std::istream &is, vec3 &t)
{
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return  is;
}

inline std::ostream& operator<< (std::ostream &os, const vec3 &t)
{
	os << t.e[0] << " " << t.e[1] << t.e[2];
	return os;
}

/////////////////////////
//general operators

inline vec3 operator+ (const vec3 &v1, const vec3 &v2)
{
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator- (const vec3 &v1, const vec3 &v2)
{
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator* (const vec3 &v1, const vec3 &v2)
{
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/ (const vec3 &v1, const vec3 &v2)
{
	return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator* (float t, const vec3 &v)
{
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

//can divide a matrix by a scalar value, but can't divide a scalar value by a matrix
inline vec3 operator/ (vec3 v, float t)
{
	return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vec3 operator* (const vec3 &v, float t)
{
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
} 

/////////////////////////
//common linear algebra functions

//unit vector is the direction of the vector, but it's length is scaled down to 1.0
//similar concept as normalize
//turns self into a unit vector
inline void vec3::make_unit_vector()
{
	float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
}

//if you're unsure of what's going on here, look up dot product linear algebra
inline float dot(const vec3 &v1, const vec3 &v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

//linear algebra cross product
inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
	return vec3( 
		(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])
	);
}

//returns a new vector that is of unit length
inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}

/////////////////////////
//self modifying operators

inline vec3& vec3::operator+=(const vec3& v2)
{
	e[0] += v2.e[0];
	e[1] += v2.e[1];
	e[2] += v2.e[2];
	return  *this;
}

inline vec3& vec3::operator*=(const vec3& v2)
{
	e[0] *= v2.e[0];
	e[1] *= v2.e[1];
	e[2] *= v2.e[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3& v2)
{
	e[0] /= v2.e[0];
	e[1] /= v2.e[1];
	e[2] /= v2.e[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3& v2)
{
	e[0] -= v2.e[0];
	e[1] -= v2.e[1];
	e[2] -= v2.e[2];
	return  *this;
}

inline vec3& vec3::operator*=(const float t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/=(const float t)
{
	e[0] /= t;
	e[1] /= t;
	e[2] /= t;
	return *this;
}