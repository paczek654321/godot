/**************************************************************************/
/*  style_box_flat_merged.cpp                                                    */
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

#include "style_box_flat_merged.h"

#include "core/math/math_defs.h"
#include "core/math/vector2.h"
#include "core/object/class_db.h"
#include "core/string/print_string.h"
#include "core/string/string_name.h"
#include "core/variant/callable.h"
#include "core/variant/variant.h"
#include "scene/property_utils.h"

#include "scene/resources/style_box_flat.h"
#include "scene/resources/style_box_flat_shared.h"

#define stylebox_variable(name) \
void StyleBoxFlatMerged::set_##name##_stylebox(const Ref<StyleBoxFlat> &p_stylebox) { \
	Callable changed = Callable(this, "emit_changed"); \
	if (name.is_valid()) \
	{ \
		name->disconnect_changed(changed); \
	} \
	name = p_stylebox; \
	if (name.is_valid()) \
	{ \
		name->connect_changed(changed); \
	} \
	emit_changed(); \
} \
Ref<StyleBoxFlat> StyleBoxFlatMerged::get_##name##_stylebox() { \
	return name; \
} \

stylebox_variable(primary);
stylebox_variable(secondary)

#define get_valid_value(getter, property) \
if (primary.is_valid()) \
{ \
	if (!secondary.is_valid() || PropertyUtils::is_property_value_different(primary.ptr(), primary->getter, PropertyUtils::get_property_default_value(primary.ptr(), property))) \
	{ \
		return primary->getter; \
	} \
} \
if (secondary.is_valid()) \
{ \
	return secondary->getter; \
} \
return ClassDB::class_get_default_property_value("StyleBoxFlat", property);

#define tetradirectional(getter, condition, v1, v2, v3, v4) \
switch (condition) { \
	case 0: \
		get_valid_value(getter, v1); \
		break; \
	case 1: \
		get_valid_value(getter, v2); \
		break; \
	case 2: \
		get_valid_value(getter, v3); \
		break; \
	default: \
		get_valid_value(getter, v4); \
		break; \
}

Color StyleBoxFlatMerged::get_bg_color() const {
	get_valid_value(get_bg_color(), "bg_color")
}

Color StyleBoxFlatMerged::get_border_color() const {
	get_valid_value(get_border_color(), "border_color")
}

int StyleBoxFlatMerged::get_border_width_min() const {
	return MIN(MIN(get_border_width(Side(0)), get_border_width(Side(1))), MIN(get_border_width(Side(2)), get_border_width(Side(3))));
}

int StyleBoxFlatMerged::get_border_width(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0);
	tetradirectional(get_border_width(p_side), p_side, "border_width_left", "border_width_top", "border_width_right", "border_width_bottom")
}

bool StyleBoxFlatMerged::get_border_blend() const {
	get_valid_value(get_border_blend(), "border_blend")
}

int StyleBoxFlatMerged::get_corner_radius(const Corner p_corner) const {
	ERR_FAIL_INDEX_V((int)p_corner, 4, 0);
	tetradirectional(get_corner_radius(p_corner), p_corner, "corner_radius_top_left", "corner_radius_top_right", "corner_radius_bottom_right", "corner_radius_bottom_left")
}

int StyleBoxFlatMerged::get_corner_detail() const {
	get_valid_value(get_corner_detail(), "corner_detail")
}

float StyleBoxFlatMerged::get_expand_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	tetradirectional(get_expand_margin(p_side), p_side, "expand_margin_left", "expand_margin_top", "expand_margin_right", "expand_margin_bottom")
}

bool StyleBoxFlatMerged::is_draw_center_enabled() const {
	get_valid_value(is_draw_center_enabled(), "is_draw_center_enabled")
}

Vector2 StyleBoxFlatMerged::get_skew() const {
	get_valid_value(get_skew(), "skew")
}

Color StyleBoxFlatMerged::get_shadow_color() const {
	get_valid_value(get_shadow_color(), "shadow_color")
}

int StyleBoxFlatMerged::get_shadow_size() const {
	get_valid_value(get_shadow_size(), "shadow_size")
}

Point2 StyleBoxFlatMerged::get_shadow_offset() const {
	get_valid_value(get_shadow_offset(), "shadow_offset")
}

bool StyleBoxFlatMerged::is_anti_aliased() const {
	get_valid_value(is_anti_aliased(), "anti_aliasing")
}

real_t StyleBoxFlatMerged::get_aa_size() const {
	get_valid_value(get_aa_size(), "anti_aliasing_size")
}

float StyleBoxFlatMerged::get_content_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	tetradirectional(get_content_margin(p_side), p_side, "content_margin_left", "content_margin_top", "content_margin_right", "content_margin_bottom")
}

float StyleBoxFlatMerged::get_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	float margin = get_content_margin(p_side);
	if (margin < 0) {
		return get_style_margin(p_side);
	} else {
		return margin;
	}
}

Rect2 StyleBoxFlatMerged::get_draw_rect(const Rect2 &p_rect) const {
	real_t expand_margin[4] = {get_expand_margin(SIDE_LEFT),get_expand_margin(SIDE_TOP),get_expand_margin(SIDE_RIGHT),get_expand_margin(SIDE_BOTTOM)};
	return StyleBoxFlatShared::get_draw_rect(p_rect, expand_margin, get_shadow_size(), get_shadow_offset());
}

void StyleBoxFlatMerged::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	real_t border_width[4] = {(real_t)get_border_width(SIDE_LEFT),(real_t)get_border_width(SIDE_TOP),(real_t)get_border_width(SIDE_RIGHT),(real_t)get_border_width(SIDE_BOTTOM)};
	real_t expand_margin[4] = {get_expand_margin(SIDE_LEFT),get_expand_margin(SIDE_TOP),get_expand_margin(SIDE_RIGHT),get_expand_margin(SIDE_BOTTOM)};
	real_t corner_radius[4] = {(real_t)get_corner_radius(CORNER_TOP_LEFT),(real_t)get_corner_radius(CORNER_TOP_RIGHT),(real_t)get_corner_radius(CORNER_BOTTOM_RIGHT),(real_t)get_corner_radius(CORNER_BOTTOM_LEFT)};
	return StyleBoxFlatShared::draw(
		p_canvas_item,
		p_rect,
		border_width,
		get_shadow_size(),
		is_draw_center_enabled(),
		expand_margin,
		corner_radius,
		get_skew(),
		is_anti_aliased(),
		get_border_blend(),
		get_border_color(),
		get_bg_color(),
		get_aa_size(),
		get_shadow_offset(),
		get_shadow_color(),
		get_corner_detail()
	);
}

float StyleBoxFlatMerged::get_style_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	return get_border_width(p_side);
}

void StyleBoxFlatMerged::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_primary_stylebox", "stylebox"), &StyleBoxFlatMerged::set_primary_stylebox);
	ClassDB::bind_method(D_METHOD("get_primary_stylebox"), &StyleBoxFlatMerged::get_primary_stylebox);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "primary_stylebox", PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxFlat"), "set_primary_stylebox", "get_primary_stylebox");

	ClassDB::bind_method(D_METHOD("set_secondary_stylebox", "stylebox"), &StyleBoxFlatMerged::set_secondary_stylebox);
	ClassDB::bind_method(D_METHOD("get_secondary_stylebox"), &StyleBoxFlatMerged::get_secondary_stylebox);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "secondary_stylebox", PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxFlat"), "set_secondary_stylebox", "get_secondary_stylebox");
	
}