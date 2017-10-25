#pragma once

#include <stdint.h>
#include "string.hpp"

namespace vga
{
	// Careful with the memory design, this structure is shared between 32 and 64 mode
	struct tm_t
	{
		uint8_t w, h;
		uint8_t x, y;
		uint64_t vid __attribute__((aligned(8)));

		enum class bc_t : uint8_t;
		enum class fc_t	: uint8_t;

		tm_t() = default;
		tm_t(uint8_t w, uint8_t h, bc_t bc, fc_t fc, uint16_t* vid)
			: w(w), h(h), x(0), y(0), vid((uint64_t)vid), clr(0), old_pos(0)
		{
			ctrl_set_bc(bc);
			ctrl_set_fc(fc);
		}

		uint16_t get_index() const
		{
			return x +y *w;
		}

		char putchar(char const __c, uint16_t __width)
		{
			((uint16_t*)vid)[get_index()] = (((uint16_t)clr << 8) | (uint16_t)__c);
			ctrl_inc_x(__width);

			return __c;
		}

		void ctrl_set_bc(bc_t bc)
		{
			clr = (clr &B(00001111)) | uint8_t(bc);
		}

		void ctrl_set_fc(fc_t fc)
		{
			clr = (clr &B(11110000)) | uint8_t(fc);
		}

		void ctrl_inc_x(int16_t const o = 1)
		{
			(void)o;
			if (x < w)
				++x;
			else
				ctrl_nl();
		}

		void ctrl_inc_y(int16_t const o = 1)
		{
			(void)o;
			if (y < h)
				++y;
			else
				ctrl_scroll();
		}

		void ctrl_nl()
		{
			ctrl_cr();
			ctrl_lf();
		}

		void ctrl_cr()
		{
			x = 0;
		}

		void ctrl_lf()
		{
			if (++y >= h)
				ctrl_scroll();
		}

		// FIXME override with default bc
		void ctrl_scroll(int16_t const o = 1)
		{
			(void)o;
			// Copy down
			memcpy_elem<uint64_t>((uint64_t*)vid, (uint64_t*)(vid +(w << 1)), ((w *(h -1)) << 1) >> 3);
			// Clear last line, TODO use default bc color
			uint64_t c = (((uint16_t)clr << 8) | (uint16_t)' ');
			uint64_t v = (c << 48) | (c << 32) | (c << 16) | c;
			memset_elem<uint64_t>((uint64_t*)(vid +(w *h *2) -w), v, w >> 3);

			if (y)
				--y;
		}

		void ctrl_reset_pos()
		{
			x = y = 0;
		}

		// Clear screen
		void ctrl_clear()
		{
			ctrl_reset_pos();

			uint64_t c = (((uint16_t)clr << 8) | (uint16_t)' ');
			uint64_t v = (c << 48) | (c << 32) | (c << 16) | c;
			memset_elem<uint64_t>((uint64_t*)vid, v, (w *h << 1) >> 3);
		}

		enum class bc_t : uint8_t
		{
			BLACK			= 0,
			LIGHTBLUE		= 1 << 4,
			LIGHTGREEN		= 2 << 4,
			LIGHTTURQUOISE	= 3 << 4,
			LIGHTRED		= 4 << 4,
			LIGHTMAGENTA	= 5 << 4,
			ORANGE			= 6 << 4,
			WHITE			= 7 << 4,
			MOD_BLINKING	= 8 << 4	//modifier-bit for blinking symols
		};

		enum class fc_t : uint8_t
		{
			BLACK			= 0,
			BLUE			= 1,
			GREEN			= 2,
			TURQUOISE		= 3,
			RED				= 4,
			MAGENTA			= 5,
			BROWN			= 6,
			LIGHTGRAY		= 7,
			MOD_INTENSITY	= 8			//modifier-bit for more intense symols
		};

	public:
		void ctrl_update()
		{
			uint16_t pos = get_index();

			if (pos != old_pos)
			{
				old_pos = pos;

				/*system::outb(0x3D4, 0x0F);
				system::outb(0x3D5, (ubyte_t)(pos & 0xFF));
				system::outb(0x3D4, 0x0E);
				system::outb(0x3D5, (ubyte_t)((pos >> 8) & 0xFF));*/
			}
		}

		union
		{
			uint8_t clr;
			struct
			{
				fc_t fc;
				bc_t bc;
			};
		};
		uint16_t old_pos;
	} __attribute__((packed,aligned(8)));
}

static_assert(sizeof(vga::tm_t) == 3 *8, "Sizeof error");
