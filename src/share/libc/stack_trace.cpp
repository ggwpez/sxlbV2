#include "stack_trace.hpp"
#include "log.hpp"

void print_stack_trace(size_t max_frames/*F const& fptr*/)
{
	cpu_word_t* bp = &max_frames -3;

	logl("Trace:");
	for (size_t i = 0; i < max_frames; ++i)
	{
		cpu_word_t ip = bp[1];
		if (! ip)
			break;

		bp = reinterpret_cast<cpu_word_t*>(bp[0]);
		/*cpu_word_t* args = &bp[2];*/
		logl("0x%P", ip);
	}
}
