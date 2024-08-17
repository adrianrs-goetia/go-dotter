/**************************************************************************/
/*  physics_server3d_extension_motion_result.hpp                          */
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

// THIS FILE IS GENERATED. EDITS WILL BE LOST.

#ifndef GODOT_CPP_PHYSICS_SERVER3D_EXTENSION_MOTION_RESULT_HPP
#define GODOT_CPP_PHYSICS_SERVER3D_EXTENSION_MOTION_RESULT_HPP

#include <godot_cpp/classes/physics_server3d_extension_motion_collision.hpp>

namespace godot {

struct PhysicsServer3DExtensionMotionResult {
	Vector3 travel;
	Vector3 remainder;
	float collision_depth;
	float collision_safe_fraction;
	float collision_unsafe_fraction;
	PhysicsServer3DExtensionMotionCollision collisions[32];
	int collision_count;
};

GDVIRTUAL_NATIVE_PTR(PhysicsServer3DExtensionMotionResult);

} // namespace godot

#endif // ! GODOT_CPP_PHYSICS_SERVER3D_EXTENSION_MOTION_RESULT_HPP