#pragma once

#ifdef _WIN32
    // #ifdef LIBRARY_EXPORTS
        #define DD3D_API __declspec(dllexport)
    // #else
        // #define DD3D_API __declspec(dllimport)
    // #endif
#else
    #define DD3D_API
#endif

#include "../../src/debugdraw3d/common/colors.h"
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>


namespace DebugDraw{
	DD3D_API void Arrow(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.5f, const bool &is_absolute_size = false, const real_t &duration = 0);
	DD3D_API void Box(const Vector3 &position, const Quaternion &rotation, const Vector3 &size, const Color &color = Colors::empty_color, const bool &is_box_centered = false, const real_t &duration = 0);
	DD3D_API void Line(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &duration = 0);
	DD3D_API void Position(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);
	DD3D_API void Sphere(const Vector3 &position, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0);
}