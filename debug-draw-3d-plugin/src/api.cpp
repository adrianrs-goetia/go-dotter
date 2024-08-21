#include <3d/debug_draw_3d.h>
#include <debugdraw3d/api.h>


void DebugDraw::Arrow(const Vector3& a, const Vector3& b, const Color& color, const real_t& arrow_size,
		const bool& is_absolute_size, const real_t& duration) {
	DebugDraw3D::get_singleton()->draw_arrow(a, b, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw::Box(const Vector3& position, const Quaternion& rotation, const Vector3& size, const Color& color,
		const bool& is_box_centered, const real_t& duration) {
	DebugDraw3D::get_singleton()->draw_box(position, rotation, size, color, is_box_centered, duration);
}

void DebugDraw::Line(const Vector3& a, const Vector3& b, const Color& color, const real_t& duration) {
	DebugDraw3D::get_singleton()->draw_line(a, b, color, duration);
}

void DebugDraw::Position(const Transform3D& transform, const Color& color, const real_t& duration) {
	DebugDraw3D::get_singleton()->draw_position(transform, color, duration);
}

void DebugDraw::Sphere(const Vector3& position, const real_t& radius, const Color& color, const real_t& duration) {
	DebugDraw3D::get_singleton()->draw_sphere(position, radius, color, duration);
}
