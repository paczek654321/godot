/**************************************************************************/
/*  style_box_flat.cpp                                                    */
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

#include "style_box_flat.h"

#include "core/config/engine.h"

#include "core/math/math_defs.h"
#include "scene/resources/style_box_flat_shared.h"

float StyleBoxFlat::get_style_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	return border_width[p_side];
}

void StyleBoxFlat::_validate_property(PropertyInfo &p_property) const {
	if (!Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	if (!anti_aliased && p_property.name == "anti_aliasing_size") {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}
}

void StyleBoxFlat::set_bg_color(const Color &p_color) {
	bg_color = p_color;
	emit_changed();
}

Color StyleBoxFlat::get_bg_color() const {
	return bg_color;
}

void StyleBoxFlat::set_border_color(const Color &p_color) {
	border_color = p_color;
	emit_changed();
}

Color StyleBoxFlat::get_border_color() const {
	return border_color;
}

void StyleBoxFlat::set_border_width_all(int p_size) {
	border_width[0] = p_size;
	border_width[1] = p_size;
	border_width[2] = p_size;
	border_width[3] = p_size;
	emit_changed();
}

int StyleBoxFlat::get_border_width_min() const {
	return MIN(MIN(border_width[0], border_width[1]), MIN(border_width[2], border_width[3]));
}

void StyleBoxFlat::set_border_width(Side p_side, int p_width) {
	ERR_FAIL_INDEX((int)p_side, 4);
	border_width[p_side] = p_width;
	emit_changed();
}

int StyleBoxFlat::get_border_width(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0);
	return border_width[p_side];
}

void StyleBoxFlat::set_border_blend(bool p_blend) {
	blend_border = p_blend;
	emit_changed();
}

bool StyleBoxFlat::get_border_blend() const {
	return blend_border;
}

void StyleBoxFlat::set_corner_radius(const Corner p_corner, const int radius) {
	ERR_FAIL_INDEX((int)p_corner, 4);
	corner_radius[p_corner] = radius;
	emit_changed();
}

void StyleBoxFlat::set_corner_radius_all(int radius) {
	for (int i = 0; i < 4; i++) {
		corner_radius[i] = radius;
	}

	emit_changed();
}

void StyleBoxFlat::set_corner_radius_individual(const int radius_top_left, const int radius_top_right, const int radius_bottom_right, const int radius_bottom_left) {
	corner_radius[0] = radius_top_left;
	corner_radius[1] = radius_top_right;
	corner_radius[2] = radius_bottom_right;
	corner_radius[3] = radius_bottom_left;

	emit_changed();
}

int StyleBoxFlat::get_corner_radius(const Corner p_corner) const {
	ERR_FAIL_INDEX_V((int)p_corner, 4, 0);
	return corner_radius[p_corner];
}

void StyleBoxFlat::set_corner_detail(const int &p_corner_detail) {
	corner_detail = CLAMP(p_corner_detail, 1, 20);
	emit_changed();
}

int StyleBoxFlat::get_corner_detail() const {
	return corner_detail;
}

void StyleBoxFlat::set_expand_margin(Side p_side, float p_size) {
	ERR_FAIL_INDEX((int)p_side, 4);
	expand_margin[p_side] = p_size;
	emit_changed();
}

void StyleBoxFlat::set_expand_margin_all(float p_expand_margin_size) {
	for (int i = 0; i < 4; i++) {
		expand_margin[i] = p_expand_margin_size;
	}
	emit_changed();
}

void StyleBoxFlat::set_expand_margin_individual(float p_left, float p_top, float p_right, float p_bottom) {
	expand_margin[SIDE_LEFT] = p_left;
	expand_margin[SIDE_TOP] = p_top;
	expand_margin[SIDE_RIGHT] = p_right;
	expand_margin[SIDE_BOTTOM] = p_bottom;
	emit_changed();
}

float StyleBoxFlat::get_expand_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	return expand_margin[p_side];
}

void StyleBoxFlat::set_draw_center(bool p_enabled) {
	draw_center = p_enabled;
	emit_changed();
}

bool StyleBoxFlat::is_draw_center_enabled() const {
	return draw_center;
}

void StyleBoxFlat::set_skew(Vector2 p_skew) {
	skew = p_skew;
	emit_changed();
}

Vector2 StyleBoxFlat::get_skew() const {
	return skew;
}

void StyleBoxFlat::set_shadow_color(const Color &p_color) {
	shadow_color = p_color;
	emit_changed();
}

Color StyleBoxFlat::get_shadow_color() const {
	return shadow_color;
}

void StyleBoxFlat::set_shadow_size(const int &p_size) {
	shadow_size = p_size;
	emit_changed();
}

int StyleBoxFlat::get_shadow_size() const {
	return shadow_size;
}

void StyleBoxFlat::set_shadow_offset(const Point2 &p_offset) {
	shadow_offset = p_offset;
	emit_changed();
}

Point2 StyleBoxFlat::get_shadow_offset() const {
	return shadow_offset;
}

void StyleBoxFlat::set_anti_aliased(const bool &p_anti_aliased) {
	anti_aliased = p_anti_aliased;
	emit_changed();
	notify_property_list_changed();
}

bool StyleBoxFlat::is_anti_aliased() const {
	return anti_aliased;
}

void StyleBoxFlat::set_aa_size(const real_t p_aa_size) {
	aa_size = CLAMP(p_aa_size, 0.01, 10);
	emit_changed();
}

real_t StyleBoxFlat::get_aa_size() const {
	return aa_size;
}

Rect2 StyleBoxFlat::get_draw_rect(const Rect2 &p_rect) const {
	return StyleBoxFlatShared::get_draw_rect(p_rect, expand_margin, shadow_size, shadow_offset);
}

void StyleBoxFlat::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	return StyleBoxFlatShared::draw(
		p_canvas_item,
		p_rect,
		border_width,
		shadow_size,
		draw_center,
		expand_margin,
		corner_radius,
		skew,
		anti_aliased,
		blend_border,
		border_color,
		bg_color,
		aa_size,
		shadow_offset,
		shadow_color,
		corner_detail
	);
}

void StyleBoxFlat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_bg_color", "color"), &StyleBoxFlat::set_bg_color);
	ClassDB::bind_method(D_METHOD("get_bg_color"), &StyleBoxFlat::get_bg_color);

	ClassDB::bind_method(D_METHOD("set_border_color", "color"), &StyleBoxFlat::set_border_color);
	ClassDB::bind_method(D_METHOD("get_border_color"), &StyleBoxFlat::get_border_color);

	ClassDB::bind_method(D_METHOD("set_border_width_all", "width"), &StyleBoxFlat::set_border_width_all);
	ClassDB::bind_method(D_METHOD("get_border_width_min"), &StyleBoxFlat::get_border_width_min);

	ClassDB::bind_method(D_METHOD("set_border_width", "margin", "width"), &StyleBoxFlat::set_border_width);
	ClassDB::bind_method(D_METHOD("get_border_width", "margin"), &StyleBoxFlat::get_border_width);

	ClassDB::bind_method(D_METHOD("set_border_blend", "blend"), &StyleBoxFlat::set_border_blend);
	ClassDB::bind_method(D_METHOD("get_border_blend"), &StyleBoxFlat::get_border_blend);

	ClassDB::bind_method(D_METHOD("set_corner_radius_all", "radius"), &StyleBoxFlat::set_corner_radius_all);

	ClassDB::bind_method(D_METHOD("set_corner_radius", "corner", "radius"), &StyleBoxFlat::set_corner_radius);
	ClassDB::bind_method(D_METHOD("get_corner_radius", "corner"), &StyleBoxFlat::get_corner_radius);

	ClassDB::bind_method(D_METHOD("set_expand_margin", "margin", "size"), &StyleBoxFlat::set_expand_margin);
	ClassDB::bind_method(D_METHOD("set_expand_margin_all", "size"), &StyleBoxFlat::set_expand_margin_all);
	ClassDB::bind_method(D_METHOD("get_expand_margin", "margin"), &StyleBoxFlat::get_expand_margin);

	ClassDB::bind_method(D_METHOD("set_draw_center", "draw_center"), &StyleBoxFlat::set_draw_center);
	ClassDB::bind_method(D_METHOD("is_draw_center_enabled"), &StyleBoxFlat::is_draw_center_enabled);

	ClassDB::bind_method(D_METHOD("set_skew", "skew"), &StyleBoxFlat::set_skew);
	ClassDB::bind_method(D_METHOD("get_skew"), &StyleBoxFlat::get_skew);

	ClassDB::bind_method(D_METHOD("set_shadow_color", "color"), &StyleBoxFlat::set_shadow_color);
	ClassDB::bind_method(D_METHOD("get_shadow_color"), &StyleBoxFlat::get_shadow_color);

	ClassDB::bind_method(D_METHOD("set_shadow_size", "size"), &StyleBoxFlat::set_shadow_size);
	ClassDB::bind_method(D_METHOD("get_shadow_size"), &StyleBoxFlat::get_shadow_size);

	ClassDB::bind_method(D_METHOD("set_shadow_offset", "offset"), &StyleBoxFlat::set_shadow_offset);
	ClassDB::bind_method(D_METHOD("get_shadow_offset"), &StyleBoxFlat::get_shadow_offset);

	ClassDB::bind_method(D_METHOD("set_anti_aliased", "anti_aliased"), &StyleBoxFlat::set_anti_aliased);
	ClassDB::bind_method(D_METHOD("is_anti_aliased"), &StyleBoxFlat::is_anti_aliased);

	ClassDB::bind_method(D_METHOD("set_aa_size", "size"), &StyleBoxFlat::set_aa_size);
	ClassDB::bind_method(D_METHOD("get_aa_size"), &StyleBoxFlat::get_aa_size);

	ClassDB::bind_method(D_METHOD("set_corner_detail", "detail"), &StyleBoxFlat::set_corner_detail);
	ClassDB::bind_method(D_METHOD("get_corner_detail"), &StyleBoxFlat::get_corner_detail);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "bg_color"), "set_bg_color", "get_bg_color");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_center"), "set_draw_center", "is_draw_center_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "skew"), "set_skew", "get_skew");

	ADD_GROUP("Border Width", "border_width_");
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "border_width_left", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_border_width", "get_border_width", SIDE_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "border_width_top", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_border_width", "get_border_width", SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "border_width_right", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_border_width", "get_border_width", SIDE_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "border_width_bottom", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_border_width", "get_border_width", SIDE_BOTTOM);

	ADD_GROUP("Border", "border_");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "border_color"), "set_border_color", "get_border_color");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "border_blend"), "set_border_blend", "get_border_blend");

	ADD_GROUP("Corner Radius", "corner_radius_");
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "corner_radius_top_left", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_corner_radius", "get_corner_radius", CORNER_TOP_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "corner_radius_top_right", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_corner_radius", "get_corner_radius", CORNER_TOP_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "corner_radius_bottom_right", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_corner_radius", "get_corner_radius", CORNER_BOTTOM_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "corner_radius_bottom_left", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_corner_radius", "get_corner_radius", CORNER_BOTTOM_LEFT);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "corner_detail", PROPERTY_HINT_RANGE, "1,20,1"), "set_corner_detail", "get_corner_detail");

	ADD_GROUP("Expand Margins", "expand_margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_left", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_expand_margin", "get_expand_margin", SIDE_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_top", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_expand_margin", "get_expand_margin", SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_right", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_expand_margin", "get_expand_margin", SIDE_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_bottom", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_expand_margin", "get_expand_margin", SIDE_BOTTOM);

	ADD_GROUP("Shadow", "shadow_");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "shadow_color"), "set_shadow_color", "get_shadow_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shadow_size", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_shadow_size", "get_shadow_size");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "shadow_offset", PROPERTY_HINT_NONE, "suffix:px"), "set_shadow_offset", "get_shadow_offset");

	ADD_GROUP("Anti Aliasing", "anti_aliasing_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "anti_aliasing"), "set_anti_aliased", "is_anti_aliased");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "anti_aliasing_size", PROPERTY_HINT_RANGE, "0.01,10,0.001,suffix:px"), "set_aa_size", "get_aa_size");
}
