/**************************************************************************/
/*  style_box_flat_merged.h                                                      */
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

#pragma once

#include "scene/resources/style_box.h"
#include "scene/resources/style_box_flat.h"

class StyleBoxFlatMerged : public StyleBox {
	GDCLASS(StyleBoxFlatMerged, StyleBox);

	Ref<StyleBoxFlat> primary;
	Ref<StyleBoxFlat> secondary;

	protected:
		static void _bind_methods();
		virtual float get_style_margin(Side p_side) const override;

	public:
		void set_primary_stylebox(const Ref<StyleBoxFlat> &p_stylebox);
		Ref<StyleBoxFlat> get_primary_stylebox();

		void set_secondary_stylebox(const Ref<StyleBoxFlat> &p_stylebox);
		Ref<StyleBoxFlat> get_secondary_stylebox();

		Color get_bg_color() const;

		Color get_border_color() const;

		int get_border_width_min() const;

		int get_border_width(Side p_side) const ;

		bool get_border_blend() const;

		int get_corner_radius(Corner p_corner) const;

		int get_corner_detail() const;

		float get_expand_margin(Side p_expand_side) const;

		bool is_draw_center_enabled() const;

		Vector2 get_skew() const;

		Color get_shadow_color() const;

		int get_shadow_size() const;

		Point2 get_shadow_offset() const;

		bool is_anti_aliased() const;

		real_t get_aa_size() const;

		virtual float get_content_margin(Side p_side) const override;

		virtual float get_margin(Side p_side) const override;

		virtual Rect2 get_draw_rect(const Rect2 &p_rect) const override;
		virtual void draw(RID p_canvas_item, const Rect2 &p_rect) const override;
};
