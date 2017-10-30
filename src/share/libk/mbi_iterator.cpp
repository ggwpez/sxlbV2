#include "mbi_iterator.hpp"
#include "assert.hpp"

mbi_iterator::mbi_iterator(const void* mbi)
	: tag((multiboot_tag const*)((cpu_word_t)mbi +8))
{
	assert(! ((cpu_word_t)mbi &7));
}

multiboot_tag const* mbi_iterator::operator *() const
{
	if (tag->type == MULTIBOOT_TAG_TYPE_END)
		return nullptr;
	else
		return tag;
}

mbi_iterator& mbi_iterator::operator ++(int)	// post
{
	if (tag->type != MULTIBOOT_TAG_TYPE_END)
		tag = (multiboot_tag const*)((multiboot_uint8_t const*)tag +((tag->size +7) & ~7));

	return *this;
}
