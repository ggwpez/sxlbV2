#include <stdint.h>
#include "log.hpp"
#include "vga.hpp"
#include "textmode.hpp"

extern "C" void stage2_main(uint32_t args_start, vga::tm_t tm);

// Can there be a better way to start a kernel than this beauty	?
template<typename T>
constexpr T const& arg_cast(T const& arg)
{
	return reinterpret_cast<T const&>(*(reinterpret_cast<char const*>(&arg) +20));
}

void stage2_main(uint32_t magic, vga::tm_t tm)
{
	magic = arg_cast(magic);
	vga::tm_t cpy = arg_cast(tm);

	vga::init();
	// Just pretend we are doing some fancy magic number checking
	logl("Magic '0x%X' is %s\n", magic, (magic == STAGE2_MAGIC) ? "OK" : "INVALID");
	//logl("tm: ", cpy.vid);

	while (1);
}


