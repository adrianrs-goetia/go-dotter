/**************************************************************************/
/*  quaternion.hpp                                                        */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef GODOT_QUATERNION_HPP
#define GODOT_QUATERNION_HPP

#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot {

struct _NO_DISCARD_ Quaternion {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float components[4] = { 0, 0, 0, 1.0 };
	};

	_FORCE_INLINE_ float &operator[](int idx) {
		return components[idx];
	}
	_FORCE_INLINE_ const float &operator[](int idx) const {
		return components[idx];
	}
	_FORCE_INLINE_ float length_squared() const;
	bool is_equal_approx(const Quaternion &p_quaternion) const;
	bool is_finite() const;
	float length() const;
	void normalize();
	Quaternion normalized() const;
	bool is_normalized() const;
	Quaternion inverse() const;
	Quaternion log() const;
	Quaternion exp() const;
	_FORCE_INLINE_ float dot(const Quaternion &p_q) const;
	float angle_to(const Quaternion &p_to) const;

	Vector3 get_euler_xyz() const;
	Vector3 get_euler_yxz() const;
	Vector3 get_euler() const { return get_euler_yxz(); }

	Quaternion slerp(const Quaternion &p_to, const float &p_weight) const;
	Quaternion slerpni(const Quaternion &p_to, const float &p_weight) const;
	Quaternion spherical_cubic_interpolate(const Quaternion &p_b, const Quaternion &p_pre_a, const Quaternion &p_post_b, const float &p_weight) const;
	Quaternion spherical_cubic_interpolate_in_time(const Quaternion &p_b, const Quaternion &p_pre_a, const Quaternion &p_post_b, const float &p_weight, const float &p_b_t, const float &p_pre_a_t, const float &p_post_b_t) const;

	Vector3 get_axis() const;
	float get_angle() const;

	_FORCE_INLINE_ void get_axis_angle(Vector3 &r_axis, float &r_angle) const {
		r_angle = 2 * Math::acos(w);
		float r = ((float)1) / Math::sqrt(1 - w * w);
		r_axis.x = x * r;
		r_axis.y = y * r;
		r_axis.z = z * r;
	}

	void operator*=(const Quaternion &p_q);
	Quaternion operator*(const Quaternion &p_q) const;

	_FORCE_INLINE_ Vector3 xform(const Vector3 &v) const {
#ifdef MATH_CHECKS
		ERR_FAIL_COND_V_MSG(!is_normalized(), v, "The quaternion must be normalized.");
#endif
		Vector3 u(x, y, z);
		Vector3 uv = u.cross(v);
		return v + ((uv * w) + u.cross(uv)) * ((float)2);
	}

	_FORCE_INLINE_ Vector3 xform_inv(const Vector3 &v) const {
		return inverse().xform(v);
	}

	_FORCE_INLINE_ void operator+=(const Quaternion &p_q);
	_FORCE_INLINE_ void operator-=(const Quaternion &p_q);
	_FORCE_INLINE_ void operator*=(const float &s);
	_FORCE_INLINE_ void operator/=(const float &s);
	_FORCE_INLINE_ Quaternion operator+(const Quaternion &q2) const;
	_FORCE_INLINE_ Quaternion operator-(const Quaternion &q2) const;
	_FORCE_INLINE_ Quaternion operator-() const;
	_FORCE_INLINE_ Quaternion operator*(const float &s) const;
	_FORCE_INLINE_ Quaternion operator/(const float &s) const;

	_FORCE_INLINE_ bool operator==(const Quaternion &p_quaternion) const;
	_FORCE_INLINE_ bool operator!=(const Quaternion &p_quaternion) const;

	operator String() const;

	_FORCE_INLINE_ Quaternion() {}

	_FORCE_INLINE_ Quaternion(float p_x, float p_y, float p_z, float p_w) :
			x(p_x),
			y(p_y),
			z(p_z),
			w(p_w) {
	}

	Quaternion(const Vector3 &p_axis, float p_angle);

	Quaternion(const Vector3 &p_euler);

	Quaternion(const Quaternion &p_q) :
			x(p_q.x),
			y(p_q.y),
			z(p_q.z),
			w(p_q.w) {
	}

	void operator=(const Quaternion &p_q) {
		x = p_q.x;
		y = p_q.y;
		z = p_q.z;
		w = p_q.w;
	}

	Quaternion(const Vector3 &v0, const Vector3 &v1) { // Shortest arc.
		Vector3 c = v0.cross(v1);
		float d = v0.dot(v1);

		if (d < -1.0f + (float)CMP_EPSILON) {
			x = 0;
			y = 1;
			z = 0;
			w = 0;
		} else {
			float s = Math::sqrt((1.0f + d) * 2.0f);
			float rs = 1.0f / s;

			x = c.x * rs;
			y = c.y * rs;
			z = c.z * rs;
			w = s * 0.5f;
		}
	}
};

float Quaternion::dot(const Quaternion &p_q) const {
	return x * p_q.x + y * p_q.y + z * p_q.z + w * p_q.w;
}

float Quaternion::length_squared() const {
	return dot(*this);
}

void Quaternion::operator+=(const Quaternion &p_q) {
	x += p_q.x;
	y += p_q.y;
	z += p_q.z;
	w += p_q.w;
}

void Quaternion::operator-=(const Quaternion &p_q) {
	x -= p_q.x;
	y -= p_q.y;
	z -= p_q.z;
	w -= p_q.w;
}

void Quaternion::operator*=(const float &s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

void Quaternion::operator/=(const float &s) {
	*this *= 1.0f / s;
}

Quaternion Quaternion::operator+(const Quaternion &q2) const {
	const Quaternion &q1 = *this;
	return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
}

Quaternion Quaternion::operator-(const Quaternion &q2) const {
	const Quaternion &q1 = *this;
	return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
}

Quaternion Quaternion::operator-() const {
	const Quaternion &q2 = *this;
	return Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
}

Quaternion Quaternion::operator*(const float &s) const {
	return Quaternion(x * s, y * s, z * s, w * s);
}

Quaternion Quaternion::operator/(const float &s) const {
	return *this * (1.0f / s);
}

bool Quaternion::operator==(const Quaternion &p_quaternion) const {
	return x == p_quaternion.x && y == p_quaternion.y && z == p_quaternion.z && w == p_quaternion.w;
}

bool Quaternion::operator!=(const Quaternion &p_quaternion) const {
	return x != p_quaternion.x || y != p_quaternion.y || z != p_quaternion.z || w != p_quaternion.w;
}

_FORCE_INLINE_ Quaternion operator*(const float &p_real, const Quaternion &p_quaternion) {
	return p_quaternion * p_real;
}

} // namespace godot

#endif // GODOT_QUATERNION_HPP
