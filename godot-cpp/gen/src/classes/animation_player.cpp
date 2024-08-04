/**************************************************************************/
/*  animation_player.cpp                                                  */
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

#include <godot_cpp/classes/animation_player.hpp>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/engine_ptrcall.hpp>
#include <godot_cpp/core/error_macros.hpp>

namespace godot {

void AnimationPlayer::animation_set_next(const StringName &p_animation_from, const StringName &p_animation_to) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("animation_set_next")._native_ptr(), 3740211285);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_animation_from, &p_animation_to);
}

StringName AnimationPlayer::animation_get_next(const StringName &p_animation_from) const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("animation_get_next")._native_ptr(), 1965194235);
	CHECK_METHOD_BIND_RET(_gde_method_bind, StringName());
	return internal::_call_native_mb_ret<StringName>(_gde_method_bind, _owner, &p_animation_from);
}

void AnimationPlayer::set_blend_time(const StringName &p_animation_from, const StringName &p_animation_to, double p_sec) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_blend_time")._native_ptr(), 3231131886);
	CHECK_METHOD_BIND(_gde_method_bind);
	double p_sec_encoded;
	PtrToArg<double>::encode(p_sec, &p_sec_encoded);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_animation_from, &p_animation_to, &p_sec_encoded);
}

double AnimationPlayer::get_blend_time(const StringName &p_animation_from, const StringName &p_animation_to) const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_blend_time")._native_ptr(), 1958752504);
	CHECK_METHOD_BIND_RET(_gde_method_bind, 0.0);
	return internal::_call_native_mb_ret<double>(_gde_method_bind, _owner, &p_animation_from, &p_animation_to);
}

void AnimationPlayer::set_default_blend_time(double p_sec) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_default_blend_time")._native_ptr(), 373806689);
	CHECK_METHOD_BIND(_gde_method_bind);
	double p_sec_encoded;
	PtrToArg<double>::encode(p_sec, &p_sec_encoded);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_sec_encoded);
}

double AnimationPlayer::get_default_blend_time() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_default_blend_time")._native_ptr(), 1740695150);
	CHECK_METHOD_BIND_RET(_gde_method_bind, 0.0);
	return internal::_call_native_mb_ret<double>(_gde_method_bind, _owner);
}

void AnimationPlayer::play(const StringName &p_name, double p_custom_blend, double p_custom_speed, bool p_from_end) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("play")._native_ptr(), 3118260607);
	CHECK_METHOD_BIND(_gde_method_bind);
	double p_custom_blend_encoded;
	PtrToArg<double>::encode(p_custom_blend, &p_custom_blend_encoded);
	double p_custom_speed_encoded;
	PtrToArg<double>::encode(p_custom_speed, &p_custom_speed_encoded);
	int8_t p_from_end_encoded;
	PtrToArg<bool>::encode(p_from_end, &p_from_end_encoded);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_name, &p_custom_blend_encoded, &p_custom_speed_encoded, &p_from_end_encoded);
}

void AnimationPlayer::play_backwards(const StringName &p_name, double p_custom_blend) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("play_backwards")._native_ptr(), 2787282401);
	CHECK_METHOD_BIND(_gde_method_bind);
	double p_custom_blend_encoded;
	PtrToArg<double>::encode(p_custom_blend, &p_custom_blend_encoded);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_name, &p_custom_blend_encoded);
}

void AnimationPlayer::pause() {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("pause")._native_ptr(), 3218959716);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner);
}

void AnimationPlayer::stop(bool p_keep_state) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("stop")._native_ptr(), 107499316);
	CHECK_METHOD_BIND(_gde_method_bind);
	int8_t p_keep_state_encoded;
	PtrToArg<bool>::encode(p_keep_state, &p_keep_state_encoded);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_keep_state_encoded);
}

bool AnimationPlayer::is_playing() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("is_playing")._native_ptr(), 36873697);
	CHECK_METHOD_BIND_RET(_gde_method_bind, false);
	return internal::_call_native_mb_ret<int8_t>(_gde_method_bind, _owner);
}

void AnimationPlayer::set_current_animation(const String &p_animation) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_current_animation")._native_ptr(), 83702148);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_animation);
}

String AnimationPlayer::get_current_animation() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_current_animation")._native_ptr(), 201670096);
	CHECK_METHOD_BIND_RET(_gde_method_bind, String());
	return internal::_call_native_mb_ret<String>(_gde_method_bind, _owner);
}

void AnimationPlayer::set_assigned_animation(const String &p_animation) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_assigned_animation")._native_ptr(), 83702148);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_animation);
}

String AnimationPlayer::get_assigned_animation() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_assigned_animation")._native_ptr(), 201670096);
	CHECK_METHOD_BIND_RET(_gde_method_bind, String());
	return internal::_call_native_mb_ret<String>(_gde_method_bind, _owner);
}

void AnimationPlayer::queue(const StringName &p_name) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("queue")._native_ptr(), 3304788590);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_name);
}

PackedStringArray AnimationPlayer::get_queue() {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_queue")._native_ptr(), 2981934095);
	CHECK_METHOD_BIND_RET(_gde_method_bind, PackedStringArray());
	return internal::_call_native_mb_ret<PackedStringArray>(_gde_method_bind, _owner);
}

void AnimationPlayer::clear_queue() {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("clear_queue")._native_ptr(), 3218959716);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner);
}

void AnimationPlayer::set_speed_scale(double p_speed) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_speed_scale")._native_ptr(), 373806689);
	CHECK_METHOD_BIND(_gde_method_bind);
	double p_speed_encoded;
	PtrToArg<double>::encode(p_speed, &p_speed_encoded);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_speed_encoded);
}

double AnimationPlayer::get_speed_scale() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_speed_scale")._native_ptr(), 1740695150);
	CHECK_METHOD_BIND_RET(_gde_method_bind, 0.0);
	return internal::_call_native_mb_ret<double>(_gde_method_bind, _owner);
}

double AnimationPlayer::get_playing_speed() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_playing_speed")._native_ptr(), 1740695150);
	CHECK_METHOD_BIND_RET(_gde_method_bind, 0.0);
	return internal::_call_native_mb_ret<double>(_gde_method_bind, _owner);
}

void AnimationPlayer::set_autoplay(const String &p_name) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_autoplay")._native_ptr(), 83702148);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_name);
}

String AnimationPlayer::get_autoplay() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_autoplay")._native_ptr(), 201670096);
	CHECK_METHOD_BIND_RET(_gde_method_bind, String());
	return internal::_call_native_mb_ret<String>(_gde_method_bind, _owner);
}

void AnimationPlayer::set_movie_quit_on_finish_enabled(bool p_enabled) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_movie_quit_on_finish_enabled")._native_ptr(), 2586408642);
	CHECK_METHOD_BIND(_gde_method_bind);
	int8_t p_enabled_encoded;
	PtrToArg<bool>::encode(p_enabled, &p_enabled_encoded);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_enabled_encoded);
}

bool AnimationPlayer::is_movie_quit_on_finish_enabled() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("is_movie_quit_on_finish_enabled")._native_ptr(), 36873697);
	CHECK_METHOD_BIND_RET(_gde_method_bind, false);
	return internal::_call_native_mb_ret<int8_t>(_gde_method_bind, _owner);
}

double AnimationPlayer::get_current_animation_position() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_current_animation_position")._native_ptr(), 1740695150);
	CHECK_METHOD_BIND_RET(_gde_method_bind, 0.0);
	return internal::_call_native_mb_ret<double>(_gde_method_bind, _owner);
}

double AnimationPlayer::get_current_animation_length() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_current_animation_length")._native_ptr(), 1740695150);
	CHECK_METHOD_BIND_RET(_gde_method_bind, 0.0);
	return internal::_call_native_mb_ret<double>(_gde_method_bind, _owner);
}

void AnimationPlayer::seek(double p_seconds, bool p_update, bool p_update_only) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("seek")._native_ptr(), 1807872683);
	CHECK_METHOD_BIND(_gde_method_bind);
	double p_seconds_encoded;
	PtrToArg<double>::encode(p_seconds, &p_seconds_encoded);
	int8_t p_update_encoded;
	PtrToArg<bool>::encode(p_update, &p_update_encoded);
	int8_t p_update_only_encoded;
	PtrToArg<bool>::encode(p_update_only, &p_update_only_encoded);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_seconds_encoded, &p_update_encoded, &p_update_only_encoded);
}

void AnimationPlayer::set_process_callback(AnimationPlayer::AnimationProcessCallback p_mode) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_process_callback")._native_ptr(), 1663839457);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_mode);
}

AnimationPlayer::AnimationProcessCallback AnimationPlayer::get_process_callback() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_process_callback")._native_ptr(), 4207496604);
	CHECK_METHOD_BIND_RET(_gde_method_bind, AnimationPlayer::AnimationProcessCallback(0));
	return (AnimationPlayer::AnimationProcessCallback)internal::_call_native_mb_ret<int64_t>(_gde_method_bind, _owner);
}

void AnimationPlayer::set_method_call_mode(AnimationPlayer::AnimationMethodCallMode p_mode) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_method_call_mode")._native_ptr(), 3413514846);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_mode);
}

AnimationPlayer::AnimationMethodCallMode AnimationPlayer::get_method_call_mode() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_method_call_mode")._native_ptr(), 3583380054);
	CHECK_METHOD_BIND_RET(_gde_method_bind, AnimationPlayer::AnimationMethodCallMode(0));
	return (AnimationPlayer::AnimationMethodCallMode)internal::_call_native_mb_ret<int64_t>(_gde_method_bind, _owner);
}

void AnimationPlayer::set_root(const NodePath &p_path) {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("set_root")._native_ptr(), 1348162250);
	CHECK_METHOD_BIND(_gde_method_bind);
	internal::_call_native_mb_no_ret(_gde_method_bind, _owner, &p_path);
}

NodePath AnimationPlayer::get_root() const {
	static GDExtensionMethodBindPtr _gde_method_bind = internal::gdextension_interface_classdb_get_method_bind(AnimationPlayer::get_class_static()._native_ptr(), StringName("get_root")._native_ptr(), 4075236667);
	CHECK_METHOD_BIND_RET(_gde_method_bind, NodePath());
	return internal::_call_native_mb_ret<NodePath>(_gde_method_bind, _owner);
}


} // namespace godot 