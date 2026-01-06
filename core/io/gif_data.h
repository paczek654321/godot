#include "core/io/image.h"
#include "core/io/resource.h"
#include "core/math/vector2.h"
#include "core/math/vector2i.h"
#include "core/object/object.h"
#include "core/templates/vector.h"
#include "core/variant/typed_array.h"
#include "core/variant/variant.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>

class GIFData : public Resource {
	GDCLASS(GIFData, Resource);

	protected: static void _bind_methods();

	public:
		struct GIFFrame {
			enum DisposalMethod {
				no_disposal,
				keep,
				restore_bg,
				restore_previous
			};
			
			bool transparent;
			DisposalMethod disposal_method;
			
			uint16_t display_time;
			int transparent_idx;

			Vector2i offset;
			Vector2i size;
			
			int lct_size;
			bool lct_enabled;
			PackedByteArray lct;

			int mcs;
			Vector<int> pixel_data;
		};
		struct ByteStream {
			uint32_t offset = 0;
			uint32_t bit_offset = 0;
			uint8_t* bytes;
			size_t size;

			template<typename T> T read() {
				T out = *reinterpret_cast<T*>((bytes+offset));
				offset += sizeof(T);
				return out;
			}

			uint8_t* read_bytes(size_t num) {
				uint8_t* out = bytes+offset;
				offset += num;
				return out;
			}
			
			uint16_t read_sub_byte(uint32_t bits) {
				uint16_t out = 0;

				uint32_t real_offset = (offset * 8) + bit_offset;
				uint32_t c_byte = real_offset/8;
				uint32_t c_bit = real_offset%8;

				for (uint32_t i = 0; i < bits; i++) {
					ERR_FAIL_COND_V_MSG(c_byte >= size, -1, "Corrupted file");
					if (bytes[c_byte] & (1 << c_bit)) {
						out |= 1<<i;
					}
					
					c_bit++;
					if (c_bit == 8) {
						c_bit = 0;
						c_byte++;
					}
				}
				
				offset = c_byte;
    			bit_offset = c_bit;

				return out;
			}

			void ignore_bytes(size_t num) { offset += num; }

			ByteStream(uint8_t* p_bytes, size_t p_size):
				bytes(p_bytes), size(p_size) {}

			ByteStream(Vector<uint8_t>& p_vector):
				bytes(p_vector.ptrw()), size(p_vector.size()) {}
		};

		Vector2i size;
			
		int gct_size = 0;
		bool gct_enabled = false;
		PackedByteArray gct;

		int bg_color = 0;

		Vector<GIFFrame> frames;

		PackedByteArray canvas;

		GIFData() = default;
		GIFData(PackedByteArray p_bytes);

		static Ref<GIFData> parse(PackedByteArray p_bytes);

		Ref<Image> render_frame(int idx, bool transparent_bg);

		TypedArray<Ref<Image>> render_frames(bool transparent_bg = true);

		Vector2i get_size();

		int get_frame_display_time(int idx);
};