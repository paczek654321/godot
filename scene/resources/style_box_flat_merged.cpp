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

#include "core/io/resource.h"
#include "core/math/math_defs.h"
#include "core/math/vector2.h"
#include "core/object/class_db.h"
#include "core/string/print_string.h"
#include "core/string/string_name.h"
#include "core/variant/callable.h"
#include "core/variant/variant.h"
#include "scene/main/scene_tree.h"
#include "scene/main/window.h"
#include "scene/property_utils.h"
#include "scene/resources/style_box_flat.h"
#include "servers/rendering_server.h"

#include "scene/resources/style_box_flat_shared.cpp"

#define stylebox_variable(name) \
void StyleBoxFlatMerged::set_##name##_stylebox(const Ref<StyleBoxFlat> &p_stylebox) { \
	Callable changed = create_custom_callable_function_pointer<Resource>(this, "emit_changed", &StyleBoxFlatMerged::emit_changed); \
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

#define get_valid_value(getter, fallback, property) \
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
return fallback;

#define tetradirectional(getter, fallback, condition, v1, v2, v3, v4) \
switch (condition) { \
	case 0: \
		get_valid_value(getter, fallback, v1); \
		break; \
	case 1: \
		get_valid_value(getter, fallback, v2); \
		break; \
	case 2: \
		get_valid_value(getter, fallback, v3); \
		break; \
	default: \
		get_valid_value(getter, fallback, v4); \
		break; \
}

Color StyleBoxFlatMerged::get_bg_color() const {
	get_valid_value(get_bg_color(), Color(), "bg_color")
}

Color StyleBoxFlatMerged::get_border_color() const {
	get_valid_value(get_border_color(), Color(), "border_color")
}

int StyleBoxFlatMerged::get_border_width_min() const {
	return MIN(MIN(get_border_width(Side(0)), get_border_width(Side(1))), MIN(get_border_width(Side(2)), get_border_width(Side(3))));
}

int StyleBoxFlatMerged::get_border_width(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0);
	tetradirectional(get_border_width(p_side), 0, p_side, "border_width_left", "border_width_top", "border_width_right", "border_width_bottom")
}

bool StyleBoxFlatMerged::get_border_blend() const {
	get_valid_value(get_border_blend(), false, "border_blend")
}

int StyleBoxFlatMerged::get_corner_radius(const Corner p_corner) const {
	ERR_FAIL_INDEX_V((int)p_corner, 4, 0);
	tetradirectional(get_corner_radius(p_corner), 0, p_corner, "corner_radius_top_left", "corner_radius_top_right", "corner_radius_bottom_right", "corner_radius_bottom_left")
}

int StyleBoxFlatMerged::get_corner_detail() const {
	get_valid_value(get_corner_detail(), 0, "corner_detail")
}

float StyleBoxFlatMerged::get_expand_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	tetradirectional(get_expand_margin(p_side), 0, p_side, "expand_margin_left", "expand_margin_top", "expand_margin_right", "expand_margin_bottom")
}

bool StyleBoxFlatMerged::is_draw_center_enabled() const {
	get_valid_value(is_draw_center_enabled(), true, "is_draw_center_enabled")
}

Vector2 StyleBoxFlatMerged::get_skew() const {
	get_valid_value(get_skew(), Vector2(), "skew")
}

Color StyleBoxFlatMerged::get_shadow_color() const {
	get_valid_value(get_shadow_color(), Color(), "shadow_color")
}

int StyleBoxFlatMerged::get_shadow_size() const {
	get_valid_value(get_shadow_size(), 0, "shadow_size")
}

Point2 StyleBoxFlatMerged::get_shadow_offset() const {
	get_valid_value(get_shadow_offset(), Point2(Vector2(0,0)), "shadow_offset")
}

bool StyleBoxFlatMerged::is_anti_aliased() const {
	get_valid_value(is_anti_aliased(), true, "anti_aliasing")
}

real_t StyleBoxFlatMerged::get_aa_size() const {
	get_valid_value(get_aa_size(), 0, "anti_aliasing_size")
}

float StyleBoxFlatMerged::get_content_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	tetradirectional(get_content_margin(p_side), 0, p_side, "content_margin_left", "content_margin_top", "content_margin_right", "content_margin_bottom")
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
	Rect2 draw_rect = p_rect.grow_individual(get_expand_margin(SIDE_LEFT), get_expand_margin(SIDE_TOP), get_expand_margin(SIDE_RIGHT), get_expand_margin(SIDE_BOTTOM));

	if (get_shadow_size() > 0) {
		Rect2 shadow_rect = draw_rect.grow(get_shadow_size());
		shadow_rect.position += get_shadow_offset();
		draw_rect = draw_rect.merge(shadow_rect);
	}

	return draw_rect;
}

void StyleBoxFlatMerged::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	bool draw_center = is_draw_center_enabled();
	int corner_detail = get_corner_detail();
	Color bg_color = get_bg_color();
	Vector2 skew = get_skew();
	
	bool draw_border = (get_border_width(Side(0)) > 0) || (get_border_width(Side(1)) > 0) || (get_border_width(Side(2)) > 0) || (get_border_width(Side(3)) > 0);
	bool draw_shadow = (get_shadow_size() > 0);
	if (!draw_border && !draw_center && !draw_shadow) {
		return;
	}

	Rect2 style_rect = p_rect.grow_individual(get_expand_margin(SIDE_LEFT), get_expand_margin(SIDE_TOP), get_expand_margin(SIDE_RIGHT), get_expand_margin(SIDE_BOTTOM));
	if (Math::is_zero_approx(style_rect.size.width) || Math::is_zero_approx(style_rect.size.height)) {
		return;
	}

	const bool rounded_corners = (get_corner_radius(Corner(0)) > 0) || (get_corner_radius(Corner(1)) > 0) || (get_corner_radius(Corner(2)) > 0) || (get_corner_radius(Corner(3)) > 0);
	// Only enable antialiasing if it is actually needed. This improves performance
	// and maximizes sharpness for non-skewed StyleBoxes with sharp corners.
	const bool aa_on = (rounded_corners || !get_skew().is_zero_approx()) && is_anti_aliased();

	const bool blend_on = get_border_blend() && draw_border;

	Color border_color = get_border_color();
	Color border_color_alpha = Color(border_color.r, border_color.g, border_color.b, 0);
	Color border_color_blend = (draw_center ? bg_color : border_color_alpha);
	Color border_color_inner = blend_on ? border_color_blend : border_color;

	// Adapt borders (prevent weird overlapping/glitchy drawings).
	real_t width = MAX(style_rect.size.width, 0);
	real_t height = MAX(style_rect.size.height, 0);
	real_t adapted_border[4] = { 1000000.0, 1000000.0, 1000000.0, 1000000.0 };
	
	const real_t border_width[4] = {(real_t)get_border_width(Side(0)), (real_t)get_border_width(Side(1)), (real_t)get_border_width(Side(2)), (real_t)get_border_width(Side(3))};
	adapt_values(SIDE_TOP, SIDE_BOTTOM, adapted_border, border_width, height, height, height);
	adapt_values(SIDE_LEFT, SIDE_RIGHT, adapted_border, border_width, width, width, width);

	// Adapt corners (prevent weird overlapping/glitchy drawings).
	const real_t corner_radius[4] = {(real_t)get_corner_radius(Corner(0)), (real_t)get_corner_radius(Corner(1)), (real_t)get_corner_radius(Corner(2)), (real_t)get_corner_radius(Corner(3))};
	real_t adapted_corner[4] = { 1000000.0, 1000000.0, 1000000.0, 1000000.0 };
	adapt_values(CORNER_TOP_RIGHT, CORNER_BOTTOM_RIGHT, adapted_corner, corner_radius, height, height - adapted_border[SIDE_BOTTOM], height - adapted_border[SIDE_TOP]);
	adapt_values(CORNER_TOP_LEFT, CORNER_BOTTOM_LEFT, adapted_corner, corner_radius, height, height - adapted_border[SIDE_BOTTOM], height - adapted_border[SIDE_TOP]);
	adapt_values(CORNER_TOP_LEFT, CORNER_TOP_RIGHT, adapted_corner, corner_radius, width, width - adapted_border[SIDE_RIGHT], width - adapted_border[SIDE_LEFT]);
	adapt_values(CORNER_BOTTOM_LEFT, CORNER_BOTTOM_RIGHT, adapted_corner, corner_radius, width, width - adapted_border[SIDE_RIGHT], width - adapted_border[SIDE_LEFT]);

	Rect2 infill_rect = style_rect.grow_individual(-adapted_border[SIDE_LEFT], -adapted_border[SIDE_TOP], -adapted_border[SIDE_RIGHT], -adapted_border[SIDE_BOTTOM]);

	Rect2 border_style_rect = style_rect;

	real_t aa_size_scaled = 1.0f;
	if (aa_on) {
		real_t scale_factor = 1.0f;
		const SceneTree *tree = Object::cast_to<SceneTree>(OS::get_singleton()->get_main_loop());
		if (tree) {
			const Window *window = tree->get_root();
			const Vector2 stretch_scale = window->get_stretch_transform().get_scale();
			scale_factor = MIN(stretch_scale.x, stretch_scale.y);
		}

		// Adjust AA feather size to account for the 2D scale factor, so that
		// antialiasing doesn't become blurry at viewport resolutions higher
		// than the default when using the `canvas_items` stretch mode
		// (or when using `content_scale_factor` values different than `1.0`).
		aa_size_scaled = get_aa_size() / scale_factor;
	}

	if (aa_on) {
		for (int i = 0; i < 4; i++) {
			if (border_width[i] > 0) {
				border_style_rect = border_style_rect.grow_side((Side)i, -aa_size_scaled);
			}
		}
	}

	Vector<Point2> verts;
	Vector<int> indices;
	Vector<Color> colors;
	Vector<Point2> uvs;

	// Create shadow.
	if (draw_shadow) {
		Point2 shadow_offset = get_shadow_offset();
		int shadow_size = get_shadow_size();
		Color shadow_color = get_shadow_color();

		Rect2 shadow_inner_rect = style_rect;
		shadow_inner_rect.position += shadow_offset;

		Rect2 shadow_rect = style_rect.grow(shadow_size);
		shadow_rect.position += shadow_offset;

		Color shadow_color_transparent = Color(shadow_color.r, shadow_color.g, shadow_color.b, 0);

		draw_rounded_rectangle(verts, indices, colors, shadow_inner_rect, adapted_corner,
				shadow_rect, shadow_inner_rect, shadow_color, shadow_color_transparent, corner_detail, skew);

		if (draw_center) {
			draw_rounded_rectangle(verts, indices, colors, shadow_inner_rect, adapted_corner,
					shadow_inner_rect, shadow_inner_rect, shadow_color, shadow_color, corner_detail, skew, true);
		}
	}

	// Create border (no AA).
	if (draw_border && !aa_on) {
		draw_rounded_rectangle(verts, indices, colors, border_style_rect, adapted_corner,
				border_style_rect, infill_rect, border_color_inner, border_color, corner_detail, skew);
	}

	// Create infill (no AA).
	if (draw_center && (!aa_on || blend_on)) {
		draw_rounded_rectangle(verts, indices, colors, border_style_rect, adapted_corner,
				infill_rect, infill_rect, bg_color, bg_color, corner_detail, skew, true);
	}

	if (aa_on) {
		real_t aa_border_width[4];
		real_t aa_border_width_half[4];
		real_t aa_fill_width[4];
		real_t aa_fill_width_half[4];

		if (draw_border) {
			for (int i = 0; i < 4; i++) {
				if (border_width[i] > 0) {
					aa_border_width[i] = aa_size_scaled;
					aa_border_width_half[i] = aa_size_scaled * 0.5;
					aa_fill_width[i] = 0;
					aa_fill_width_half[i] = 0;
				} else {
					aa_border_width[i] = 0;
					aa_border_width_half[i] = 0;
					aa_fill_width[i] = aa_size_scaled;
					aa_fill_width_half[i] = aa_size_scaled * 0.5;
				}
			}
		} else {
			for (int i = 0; i < 4; i++) {
				aa_border_width[i] = 0;
				aa_border_width_half[i] = 0;
				aa_fill_width[i] = aa_size_scaled;
				aa_fill_width_half[i] = aa_size_scaled * 0.5;
			}
		}

		if (draw_center) {
			// Infill rect, transparent side of antialiasing gradient (base infill rect enlarged by AA size)
			Rect2 infill_rect_aa_transparent = infill_rect.grow_individual(aa_fill_width_half[SIDE_LEFT], aa_fill_width_half[SIDE_TOP],
					aa_fill_width_half[SIDE_RIGHT], aa_fill_width_half[SIDE_BOTTOM]);
			// Infill rect, colored side of antialiasing gradient (base infill rect shrunk by AA size)
			Rect2 infill_rect_aa_colored = infill_rect_aa_transparent.grow_individual(-aa_fill_width[SIDE_LEFT], -aa_fill_width[SIDE_TOP],
					-aa_fill_width[SIDE_RIGHT], -aa_fill_width[SIDE_BOTTOM]);
			if (!blend_on) {
				// Create center fill, not antialiased yet
				draw_rounded_rectangle(verts, indices, colors, border_style_rect, adapted_corner,
						infill_rect_aa_colored, infill_rect_aa_colored, bg_color, bg_color, corner_detail, skew, true);
			}
			if (!blend_on || !draw_border) {
				Color alpha_bg = Color(bg_color.r, bg_color.g, bg_color.b, 0);
				// Add antialiasing on the center fill
				draw_rounded_rectangle(verts, indices, colors, border_style_rect, adapted_corner,
						infill_rect_aa_transparent, infill_rect_aa_colored, bg_color, alpha_bg, corner_detail, skew);
			}
		}

		if (draw_border) {
			// Inner border recct, fully colored side of antialiasing gradient (base inner rect enlarged by AA size)
			Rect2 inner_rect_aa_colored = infill_rect.grow_individual(aa_border_width_half[SIDE_LEFT], aa_border_width_half[SIDE_TOP],
					aa_border_width_half[SIDE_RIGHT], aa_border_width_half[SIDE_BOTTOM]);
			// Inner border rect, transparent side of antialiasing gradient (base inner rect shrunk by AA size)
			Rect2 inner_rect_aa_transparent = inner_rect_aa_colored.grow_individual(-aa_border_width[SIDE_LEFT], -aa_border_width[SIDE_TOP],
					-aa_border_width[SIDE_RIGHT], -aa_border_width[SIDE_BOTTOM]);
			// Outer border rect, transparent side of antialiasing gradient (base outer rect enlarged by AA size)
			Rect2 outer_rect_aa_transparent = style_rect.grow_individual(aa_border_width_half[SIDE_LEFT], aa_border_width_half[SIDE_TOP],
					aa_border_width_half[SIDE_RIGHT], aa_border_width_half[SIDE_BOTTOM]);
			// Outer border rect, colored side of antialiasing gradient (base outer rect shrunk by AA size)
			Rect2 outer_rect_aa_colored = border_style_rect.grow_individual(aa_border_width_half[SIDE_LEFT], aa_border_width_half[SIDE_TOP],
					aa_border_width_half[SIDE_RIGHT], aa_border_width_half[SIDE_BOTTOM]);

			// Create border ring, not antialiased yet
			draw_rounded_rectangle(verts, indices, colors, border_style_rect, adapted_corner,
					outer_rect_aa_colored, ((blend_on) ? infill_rect : inner_rect_aa_colored), border_color_inner, border_color, corner_detail, skew);
			if (!blend_on) {
				// Add antialiasing on the ring inner border
				draw_rounded_rectangle(verts, indices, colors, border_style_rect, adapted_corner,
						inner_rect_aa_colored, inner_rect_aa_transparent, border_color_blend, border_color, corner_detail, skew);
			}
			// Add antialiasing on the ring outer border
			draw_rounded_rectangle(verts, indices, colors, border_style_rect, adapted_corner,
					outer_rect_aa_transparent, outer_rect_aa_colored, border_color, border_color_alpha, corner_detail, skew);
		}
	}

	// Compute UV coordinates.
	Rect2 uv_rect = style_rect.grow(aa_on ? aa_size_scaled : 0);
	uvs.resize(verts.size());
	Point2 *uvs_ptr = uvs.ptrw();
	for (int i = 0; i < verts.size(); i++) {
		uvs_ptr[i].x = (verts[i].x - uv_rect.position.x) / uv_rect.size.width;
		uvs_ptr[i].y = (verts[i].y - uv_rect.position.y) / uv_rect.size.height;
	}

	// Draw stylebox.
	RenderingServer *vs = RenderingServer::get_singleton();
	vs->canvas_item_add_triangle_array(p_canvas_item, indices, verts, colors, uvs);
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