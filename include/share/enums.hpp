#pragma once

#define MAKE_ENUM_SAFE_CAST(from, to) \
	inline operator to( from const& v) \
	{ \
		assert(v < static_cast<from>(to::_size)); \
		return static_cast<to const&>(v); \
	}

#define MAKE_CAST(from, to) \
	inline operator to(from const& v) \
	{ \
		return static_cast<to const&>(v); \
	}
