/**************************************************************************/
/*  modulate_texture.cpp                                                       */
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

#include "scene/resources/modulate_texture.h"
#include "core/math/color.h"
#include "scene/resources/texture.h"

Ref<Texture2D> ModulateTexture2D::get_texture() const {
	return this->texture;
}

void ModulateTexture2D::set_texture(const Ref<Texture2D> &p_texture) {
	this->texture = p_texture;
	emit_changed();
}

Color ModulateTexture2D::get_modulate() const {
	return this->modulate;
}

void ModulateTexture2D::set_modulate(Color p_modulate) {
	this->modulate = p_modulate;
	emit_changed();
}

Ref<Image> ModulateTexture2D::get_image() const {
	if (texture.is_null()) {
		return Ref<Image>();
	}
	return this->texture->get_image();
}

int ModulateTexture2D::get_width() const {
	if (texture.is_null()) {
		return 0;
	}
	return this->texture->get_width();
}
int ModulateTexture2D::get_height() const {
	if (texture.is_null()) {
		return 0;
	}
	return this->texture->get_height();
}

RID ModulateTexture2D::get_rid() const {
	if (texture.is_null()) {
		return RS::get_singleton()->texture_2d_placeholder_create();
	}
	return this->texture->get_rid();
}

bool ModulateTexture2D::has_alpha() const {
	if (texture.is_null()) {
		return false;
	}
	return this->texture->has_alpha();
}

void ModulateTexture2D::draw(RID p_canvas_item, const Point2 &p_pos, const Color &p_modulate, bool p_transpose) const {
	if (texture.is_null()) {
		return;
	}
	this->texture->draw(p_canvas_item, p_pos, p_modulate * this->modulate, p_transpose);
}
void ModulateTexture2D::draw_rect(RID p_canvas_item, const Rect2 &p_rect, bool p_tile, const Color &p_modulate, bool p_transpose) const {
	if (texture.is_null()) {
		return;
	}
	this->texture->draw_rect(p_canvas_item, p_rect, p_tile, p_modulate * this->modulate, p_transpose);
}
void ModulateTexture2D::draw_rect_region(RID p_canvas_item, const Rect2 &p_rect, const Rect2 &p_src_rect, const Color &p_modulate, bool p_transpose, bool p_clip_uv) const {
	if (texture.is_null()) {
		return;
	}
	this->texture->draw_rect_region(p_canvas_item, p_rect, p_src_rect, p_modulate * this->modulate, p_transpose, p_clip_uv);
}

bool ModulateTexture2D::is_pixel_opaque(int p_x, int p_y) const {
	if (texture.is_null()) {
		return false;
	}
	return this->texture->is_pixel_opaque(p_x, p_y);
}

Ref<ModulateTexture2D> ModulateTexture2D::create(const Ref<Texture2D> &p_texture, Color p_modulate) {
	Ref<ModulateTexture2D> modulate_texture;
	modulate_texture.instantiate();
	modulate_texture->set_texture(p_texture);
	modulate_texture->set_modulate(p_modulate);
	return modulate_texture;
}

void ModulateTexture2D::_bind_methods() {
	ClassDB::bind_static_method("ModulateTexture2D", D_METHOD("create", "texture", "modulate"), &ModulateTexture2D::create);
	
	ClassDB::bind_method(D_METHOD("set_modulate", "modulate"), &ModulateTexture2D::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &ModulateTexture2D::get_modulate);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate"), "set_modulate", "get_modulate");

	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &ModulateTexture2D::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &ModulateTexture2D::get_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_texture", "get_texture");
}