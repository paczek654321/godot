#include "core/io/gif_data.h"
#include "core/io/image.h"
#include "core/object/class_db.h"
#include "core/object/ref_counted.h"
#include "core/string/print_string.h"
#include "core/templates/vector.h"
#include "core/variant/array.h"
#include "core/variant/variant.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct LZWState {
	Vector<int> dict[4096];
	int clear_code = 0;
	int end_of_info = 0;
	int code_size = 0;
	int next_code = 0;
	int prev_code = 0;

	LZWState(int mcs) {
		clear(mcs);
	}
	void clear(int mcs) {
		clear_code = 1<<mcs;
		end_of_info = clear_code+1;
		code_size = mcs + 1;
		next_code = end_of_info+1;
		prev_code = -1;

		for (int i = 0; i < clear_code; i++) {
			dict[i] = Vector<int>();
			dict[i].append(i);
		}
	}
};

Vector<int> lzw(uint8_t* bytes, size_t size, int mcs, int pixel_count) {
	GIFData::ByteStream bs = GIFData::ByteStream(bytes, size);
	LZWState state = LZWState(mcs);
	Vector<int> output;
	while (true) {
		if (output.size() >= pixel_count) { return output; }
		int current_code = bs.read_sub_byte(state.code_size);

		if (current_code == state.clear_code) {
			state.clear(mcs);
			current_code = bs.read_sub_byte(state.code_size);
		}

		if (current_code == state.end_of_info) { break; }

		Vector<int> out;
		if (current_code < state.next_code) {
			out = state.dict[current_code];
		} else if (current_code == state.next_code && state.prev_code != -1) {
			out = state.dict[state.prev_code].duplicate();
			out.append(out[0]);
		} else {
			continue;
		}

		output.append_array(out);

		if (state.prev_code != -1 && state.next_code < 4096) {
			state.dict[state.next_code] = state.dict[state.prev_code].duplicate();
			state.dict[state.next_code].append(out[0]);

			state.next_code += 1;

			if (state.next_code == (1 << state.code_size) && state.code_size < 12) {
				state.code_size += 1;
			}
		}
		state.prev_code = current_code;
	}
	return output;
}

GIFData::GIFData(PackedByteArray p_bytes) {
	ByteStream bs = ByteStream(p_bytes);

	bs.ignore_bytes(6); //Ignore Header
	
	size.x = bs.read<uint16_t>();
	size.y = bs.read<uint16_t>();

	uint8_t gct_packed = bs.read<uint8_t>();
	gct_size = pow(2, (gct_packed&0b00000111) + 1);
	gct_enabled = gct_packed&0b10000000;

	bg_color = bs.read<uint8_t>();

	bs.ignore_bytes(1);

	if (gct_enabled) {
		for (int i = 0; i < gct_size; i++) {
			for (int c = 0; c < 3; c++) {
				gct.push_back(bs.read<uint8_t>());
			}
		}
	}

	while (true) {
		GIFFrame frame = GIFFrame();
		uint8_t introducer = bs.read<uint8_t>();
		while (introducer == 0x21) {
			uint8_t type = bs.read<uint8_t>();

			while (true) {
				uint8_t block_size = bs.read<uint8_t>();
				if (block_size == 0) { break; }
				if (type == 0xF9) {
					uint8_t packed_flags = bs.read<uint8_t>();
					frame.transparent = packed_flags&0b00000001;
					frame.disposal_method = static_cast<GIFFrame::DisposalMethod>((packed_flags&0b00011100) >> 2);

					frame.display_time = bs.read<uint16_t>();
					frame.transparent_idx = bs.read<uint8_t>();
				} else {
					bs.ignore_bytes(block_size);
				}
			}
			introducer = bs.read<uint8_t>();
		}
		if (introducer == 0x3B) { break; }
		ERR_FAIL_COND_MSG(introducer != 0x2C, "Corrupted file");

		frame.offset.x = bs.read<uint16_t>();
		frame.offset.y = bs.read<uint16_t>();
		frame.size.x = bs.read<uint16_t>();
		frame.size.y = bs.read<uint16_t>();

		uint8_t lct_packed = bs.read<uint8_t>();
		frame.lct_size = pow(2, (lct_packed&0b00000111) + 1);
		frame.lct_enabled = lct_packed&0b10000000;

		if (frame.lct_enabled) {
			for (int i = 0; i < frame.lct_size; i++) {
				for (int c = 0; c < 3; c++) {
					frame.lct.push_back(bs.read<uint8_t>());
				}
			}
		}

		frame.mcs = bs.read<uint8_t>();

		uint32_t total_size = 0;
		Vector<uint8_t*> pointers;

		while (true) {
			uint8_t block_size = bs.read<uint8_t>();
			if (block_size == 0) { break; }
			total_size += block_size;
			pointers.push_back(bs.read_bytes(block_size));
		}

		uint8_t* bytes = reinterpret_cast<uint8_t*>(malloc(total_size));
		uint32_t offset = 0;
		for (uint8_t* ptr : pointers) {
			size_t block_size = *(ptr-1);
			memcpy(bytes+offset, ptr, block_size);
			offset += block_size;
		}

		frame.pixel_data = lzw(bytes, total_size, frame.mcs, frame.size.x*frame.size.y);
		free(bytes);

		frames.push_back(frame);
	}
}

Ref<GIFData> GIFData::parse(PackedByteArray p_bytes) {
	Ref<GIFData> ref;
	ref.instantiate(p_bytes);
	return ref;
}

Ref<Image> GIFData::render_frame(int idx, bool transparent_bg) {
	const GIFFrame& frame = frames[idx];
	const PackedByteArray& color_table = frame.lct_enabled ? frame.lct : gct;

	bool dispose = frame.disposal_method == GIFFrame::DisposalMethod::restore_bg || frame.disposal_method == GIFFrame::DisposalMethod::restore_previous;
	PackedByteArray next_canvas;
	if (dispose) { next_canvas = canvas.duplicate(); }

	int frame_start = (frame.offset.y*size.x)+frame.offset.x;
	for (int i = 0; i < frame.size.x*frame.size.y; i++) {
		int data_position = (frame_start+(i%frame.size.x)+((i/frame.size.x)*size.x))*4;
		int color = frame.pixel_data[i];

		if (frame.disposal_method == GIFFrame::DisposalMethod::restore_bg) {
			if (transparent_bg) {

			} else {
				for (int j = 0; j < 3; j++) {
					next_canvas.set(data_position+j, gct[bg_color+j]);
				}
				canvas.set(data_position+3, 255);
			}
		}

		if (frame.transparent && color == frame.transparent_idx) {
			continue;
		}

		color *= 3;
		
		for (int j = 0; j < 3; j++) {
			canvas.set(data_position+j, color_table[color+j]);
		}
		canvas.set(data_position+3, 255);
	}
	Ref<Image> img = Image::create_from_data(size.x, size.y, false, Image::FORMAT_RGBA8, canvas);
	
	if (dispose) { canvas = next_canvas; }

	return img;
}

TypedArray<Ref<Image>> GIFData::render_frames(bool transparent_bg) {
	canvas = PackedByteArray();
	canvas.resize(size.x*size.y*4);
	if (transparent_bg) {
		canvas.fill(0);
	} else {
		for (int i = 4; i < canvas.size(); i+=4) {
			for (int j = 0; j < 3; j++) {
				canvas.set(i+j, gct[bg_color+j]);
			}
			canvas.set(i+3, 255);
		}
	}

	TypedArray<Ref<Image>> out = TypedArray<Ref<Image>>();
	for (int i = 0; i < frames.size(); i++) {
		out.append(render_frame(i, transparent_bg));
	}
	return out;
}

Vector2i GIFData::get_size() {
	return size;
}

int GIFData::get_frame_display_time(int idx) {
	return frames[idx].display_time;
}

void GIFData::_bind_methods() {
	ClassDB::bind_static_method("GIFData", D_METHOD("parse", "data"), &GIFData::parse);
	ClassDB::bind_method(D_METHOD("render_frames", "transparent_bg"), &GIFData::render_frames, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("get_size"), &GIFData::get_size);
	ClassDB::bind_method(D_METHOD("get_frame_display_time", "frame"), &GIFData::get_frame_display_time);
}